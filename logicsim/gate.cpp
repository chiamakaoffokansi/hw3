#include <vector>
#include <random>
#include <iostream>
#include <sstream>
#include <fstream>

#include "../heap.h" //#include "../heap.h" //bring in your heap implementation
#include "wire.h"
#include "gate.h"
#include "circuit.h"
#include "event.h"

Circuit::Circuit() : m_current_time(0)
{
    
}

Circuit::~Circuit()
{
    for(auto i : m_wires)
    {
        delete i;
    }
    for(auto i : m_gates)
    {
        delete i;
    }
}

void Circuit::test()
{
    m_wires.push_back(new Wire(0, "input A"));
	m_wires.push_back(new Wire(1, "input B"));
	m_wires.push_back(new Wire(2, "output"));
    
    Gate* g = new And2Gate(m_wires[0], m_wires[1], m_wires[2]);
	m_gates.push_back(g);
    
	Event* e = new Event {0,m_wires[0],'0'};
	m_pq.push(e);
	
	e = new Event {0,m_wires[1],'1'};
	m_pq.push(e);
	
	e = new Event {4,m_wires[0],'1'};
	m_pq.push(e);

  e = new Event {6,m_wires[1],'0'};
	m_pq.push(e);
	
}

bool Circuit::parse(const char* fname)
{
    std::ifstream inFile(fname);
    if(!inFile)
    {
        return false;
    }
    
    std::string line;
    while( getline(inFile, line))
    {
        if(line == "WIRES")
        {
            std::string t_line;
            getline(inFile,t_line);
            int n = stoi(t_line);
            for(int i = 0;i<n;i++)
            {
                getline(inFile,t_line);
                std::stringstream ss(t_line);
                std::string s_id;
                getline(ss, s_id, ',');
                std::string s_name;
                getline(ss, s_name, ',');
                m_wires.push_back(new Wire(stoi(s_id), s_name));
            }
        }
        if(line == "GATES")
        {
            std::string t_line;
            getline(inFile,t_line);
            int n = stoi(t_line);
            for(int i = 0;i<n;i++)
            {
                getline(inFile,t_line);
                std::stringstream ss(t_line);
                std::string s_type;
                getline(ss, s_type, ',');
                if(s_type == "AND2")
                {
                    std::string s_in1;
                    getline(ss, s_in1, ',');
                    std::string s_in2;
                    getline(ss, s_in2, ',');
                    std::string s_output;
                    getline(ss, s_output, ',');
                    m_gates.push_back(new And2Gate(m_wires[stoi(s_in1)], m_wires[stoi(s_in2)], m_wires[stoi(s_output)]));
                }
                if(s_type == "OR2")
                {
                    std::string s_in1;
                    getline(ss, s_in1, ',');
                    std::string s_in2;
                    getline(ss, s_in2, ',');
                    std::string s_output;
                    getline(ss, s_output, ',');
                    m_gates.push_back(new Or2Gate(m_wires[stoi(s_in1)], m_wires[stoi(s_in2)], m_wires[stoi(s_output)]));
                }
                if(s_type == "NOT")
                {
                    std::string s_in;
                    getline(ss, s_in, ',');  // Read the input wire index
                    std::string s_output;
                    getline(ss, s_output, ',');  // Read the output wire index
                    m_gates.push_back(new NotGate(m_wires[stoi(s_in)], m_wires[stoi(s_output)]));
                }
            }
        }
        if(line == "INJECT")
        {
            std::string t_line;
            getline(inFile,t_line);
            int n = stoi(t_line);
            for(int i = 0;i<n;i++)
            {
                getline(inFile,t_line);
                std::stringstream ss(t_line);
                std::string s_time;
                getline(ss, s_time, ',');
                std::string s_wire;
                getline(ss, s_wire, ',');
                std::string s_state;
                getline(ss, s_state, ',');
            	Event* e = new Event {static_cast<uint64_t>(stoi(s_time)),m_wires[stoi(s_wire)],s_state[0]};
                //std::cout << s_time << "," << s_wire << "," << s_state << std::endl;
            	m_pq.push(e);
            }
        }
    }
    return true;
}

bool Circuit::advance(std::ostream& os)
{
	if(m_pq.size() == 0)
	{
		return false;
	}
    
    m_current_time = m_pq.top()->time;
    std::stringstream ss;
    ss << "@" << m_current_time << std::endl;
    bool updated = false;
    
    while(m_pq.top()->time == m_current_time)
    {
        if(m_pq.size() >= 1)
        {
            std::string temp = m_pq.top()->wire->setState(m_pq.top()->state, m_current_time);
            if(temp != "")
            {
                ss << temp << std::endl;
                updated = true;
            }
            delete m_pq.top();
            m_pq.pop();
            if(m_pq.size() == 0) break;
        }
        else
        {
            break;
        }
        
    }
    if(updated)
    {
        os << ss.str();
    }
    for(auto g : m_gates)
    {
        Event* e = g->update(m_current_time);
        if(e)
        {
            m_pq.push(e);
        }
    }
	return true;
}

void Circuit::run(std::ostream& os)
{
	
	while(advance(os)){
        
	}
}

void Circuit::startUml(std::ostream& os)
{
	os << "@startuml" << std::endl;
    for(auto w : m_wires)
    {
        if(w->getName().size() > 0)
				{
					os << "binary " << "\"" << w->getName() << "\"" << " as W" << w->getId() << std::endl;
				}
		}
    os << std::endl;
    os << "@0" << std::endl;
    for(auto w : m_wires)
    {
        if(w->getName().size() > 0)
				{
					os << "W" << w->getId() << " is {low,high} " << std::endl;
				}
		}
    os << std::endl;
    
}

void Circuit::endUml(std::ostream& os)
{
    os << "@enduml" << std::endl;
}

void Circuit::parse(std::ifstream& file) {
    std::string line;
    std::getline(file, line);  // Read "WIRES" section header
    std::getline(file, line);
    
    int numWires;
    file >> numWires;
    std::vector<Wire*> wires(numWires);
    
    for (int i = 0; i < numWires; ++i) {
        int wireId;
        std::string wireName;
        file >> wireId;  // Read wire ID
        file.ignore();  // Ignore comma
        std::getline(file, wireName);  // Read wire name (if present)
        
        wires[wireId] = new Wire(wireName);
    }
    
    std::getline(file, line);  // Skip to next section
    
    std::getline(file, line);    // Read "GATES" section
    std::getline(file, line);  // Skip "GATES" 
    
    int numGates;
    file >> numGates;
    file.ignore();  // intended to ignore newline after number of gates
    
    for (int i = 0; i < numGates; ++i) {
        std::string gateType;
        int inputId1, inputId2, outputId;
        
        std::getline(file, line);
        std::istringstream gateStream(line);
        gateStream >> gateType;
        
        if (gateType == "AND2" || gateType == "OR2") {
            gateStream >> inputId1;
            gateStream.ignore();  // Ignore the comma
            gateStream >> inputId2;
            gateStream.ignore();  // Ignore the comma
            gateStream >> outputId;
            
            Wire* inputWire1 = wires[inputId1];
            Wire* inputWire2 = wires[inputId2];
            Wire* outputWire = wires[outputId];
            
            if (gateType == "AND2") { //section will then interpret commands and create the cooresponding gate
                m_gates.push_back(new And2Gate(outputWire, inputWire1, inputWire2));
            } else if (gateType == "OR2") { //if or then will make or
                m_gates.push_back(new Or2Gate(outputWire, inputWire1, inputWire2));
            }
        }
        else if (gateType == "NOT") { //specfiic section for not
            gateStream >> inputId1;// will read one input wire ID and one output wire ID for NOT gate
            gateStream.ignore();  // Ignore the comma
            gateStream >> outputId;
            
            Wire* inputWire = wires[inputId1];
            Wire* outputWire = wires[outputId];
            
            m_gates.push_back(new NotGate(outputWire, inputWire)); // creates NotGate
        }
    }
    
    std::getline(file, line);  // skips to next section
    std::getline(file, line);  // skips "INJECT" section
    std::getline(file, line);  // skips blank line
    
    int numEvents;
    file >> numEvents;
    file.ignore();  // will ignore newline after the number of events
    
    for (int i = 0; i < numEvents; ++i) { //mean to read over inject section
        uint64_t eventTime;
        int wireId;
        char state;
        
        file >> eventTime;
        file.ignore();  // will ignore the comma in line
        file >> wireId;
        file.ignore();  // will ignore the comma in line
        file >> state;
        
        Wire* eventWire = wires[wireId];
        Event* newEvent = new Event{eventTime, eventWire, state};
        m_pq.push(newEvent);  // Add the event to the priority queue
    }
}