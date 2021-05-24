#include<iostream>
#include<vector>
#include<string>
#include<stack>
using namespace std;

class Transitions{
    public:
     int vertex_from; // starting state
     int vertex_to;  // ending state
     char symbol; // alphabet leading to transition
        public :
          // constructor           
          Transitions(int vertex_from, int vertex_to, char symbol){
            this->vertex_from=vertex_from;
            this->vertex_to=vertex_to;
            this->symbol=symbol;
          }
};

class NFA{
    public:
        vector<int> vertex; // states in NFA
        vector<Transitions> transitions; // transitions in NFA
        int final_state; // final state of NFA

        int get_vertex_count() {
            return vertex.size();
        }

        // setting the states of NFA
        void set_vertex(int total_vertex) {
            for(int i = 0; i < total_vertex; i++) {
                vertex.push_back(i);
            }
        }

        // setting transition rule from a state to another state
        void set_transition(int vertex_from, int vertex_to, char symbol) {
            Transitions trans(vertex_from,vertex_to,symbol);
            transitions.push_back(trans);
        }

        // setting a state as final state
        void set_final_state(int fs) {
            final_state = fs;
        }
        int get_final_state() {
            return final_state;
        }

        // printing the NFA
        void display() {
            for(int i = 0; i < transitions.size(); i++) {
                Transitions temp = transitions[i];
                cout<<"q"<<temp.vertex_from<<" -> q"<<temp.vertex_to<<" : Symbol - "<<temp.symbol<<endl;
            }
           cout<<"The final state is q"<<get_final_state()<<endl;
        }
};

    // function returning a concatenated NFA of two NFA's a and b ; eg: c = (a.b)
    NFA concat(NFA a, NFA b){
        NFA result; //resulting NFA
        result.set_vertex(a.get_vertex_count() + b.get_vertex_count());//No new state added in concatenation
        int i;
         
         // copying old transitions of NFA "a"
        for(i = 0; i < a.transitions.size(); i++) {
           Transitions new_trans = a.transitions[i];
            result.set_transition(new_trans.vertex_from, new_trans.vertex_to, new_trans.symbol); //Copy old transitions
        }

        result.set_transition(a.get_final_state(), a.get_vertex_count(), '^'); //Creating the link; final state of a will link to initial state of b

        // copying old transitions of NFA "b"
        for(i = 0; i < b.transitions.size(); i++) {
            Transitions new_trans = b.transitions[i];
            result.set_transition(new_trans.vertex_from + a.get_vertex_count(), new_trans.vertex_to + a.get_vertex_count(), new_trans.symbol);//Copy old transitions with offset as a's states have already been added
        }

        result.set_final_state(a.get_vertex_count() + b.get_vertex_count() - 1); //Mark b's final state as final state of new NFA

        return result;
    }

    // function returning a kleene closure of NFA;   eg: c = (a*)
    NFA kleene(NFA a){
        NFA result;
        int i;
        // 2 new states added because of epsilon transition
        result.set_vertex(a.get_vertex_count() + 2);
        result.set_transition(0, 1, '^'); //Epsilon transition from S0 to S1

        for(i = 0; i < a.transitions.size(); i++) {
            Transitions new_trans = a.transitions[i];
            result.set_transition(new_trans.vertex_from + 1, new_trans.vertex_to + 1, new_trans.symbol); //Copy old transitions
        }

        result.set_transition(a.get_vertex_count(), a.get_vertex_count() + 1, '^'); //Epsilon transition to new final state
        result.set_transition(a.get_vertex_count(), 1, '^'); // Reverese epsilon transition
        result.set_transition(0, a.get_vertex_count() + 1, '^'); // epsilon transition from start to final state (empty input)
        result.set_final_state(a.get_vertex_count() + 1); //Mark final state

        return result;
    }

    // function returning a union of any number of NFA's;  eg: c = (a|b) ; c is the union of 2 NFA's a and b
    NFA union_nfa(vector<NFA> selections, int no_of_selections){
        NFA result;
        int state_count = 2;
        
        for(int i = 0; i < no_of_selections; i++){
            state_count += selections[i].get_vertex_count();
        }

        result.set_vertex(state_count); // total number of states in resulting NFA
        int adder_track = 1;

        for(int i = 0; i < no_of_selections; i++){
            result.set_transition(0, adder_track, '^'); //Initial epsilon transition to the first block of 'OR'
            NFA temp = selections[i];
            for(int j = 0; j < temp.transitions.size(); j++) {
                Transitions  new_trans = temp.transitions[j];
                result.set_transition(new_trans.vertex_from + adder_track, new_trans.vertex_to + adder_track, new_trans.symbol); //Copy all transitions in first NFA
            }
            adder_track += temp.get_vertex_count(); // how many states added
            result.set_transition(adder_track - 1, state_count - 1, '^'); // Add epsilon transition to final state
        }   
        result.set_final_state(state_count - 1); //Mark final state
        return result;
    }
    
    // function to convert a regx to NFA
    NFA re_to_nfa(string re){
        stack<char> operators;
        stack<NFA> operands;
        char op_sym;
        int op_count;
        
        for(int i=0;i<re.length();i++) {
            char cur_sym = re[i];
            if(cur_sym != '(' && cur_sym != ')' && cur_sym != '*' && cur_sym != '|' && cur_sym != '.'){
                NFA new_sym; // NFA to store simple alphabet (a,b,c) ; operands in this case
                new_sym.set_vertex(2); // min 2 states
                new_sym.set_transition(0, 1, cur_sym); // intital transition
                new_sym.set_final_state(1); // setting 1 as final state
                operands.push(new_sym); //pushing it to operands for further operations
            } 
            else {
                switch (cur_sym) {
                    case '*':{ // kleene closure
                        NFA star_sym = operands.top();
                        operands.pop();
                        operands.push(kleene(star_sym));
                        break;
                        }
                    case '.': // concatenation
                        operators.push(cur_sym);
                        break;
                    case '|': // union
                        operators.push(cur_sym);
                        break;
                    case '(': // seperator; no operation
                        operators.push(cur_sym);
                        break;
                    default:
                        op_count = 0;
                        char c;
                        op_sym = operators.top(); //See which symbol is on top
                        if(op_sym == '(')
                            continue; //Keep searching operands
                        do{
                            operators.pop();
                            op_count++;
                        } while(operators.top() != '('); //Collect operands
                        operators.pop();
                        NFA op1,op2;
                        vector<NFA> selections;
                        if(op_sym == '.') {
                            for(int ii = 0; ii < op_count; ii++) {
                                op2 = operands.top();
                                operands.pop();
                                op1 = operands.top();
                                operands.pop();
                                operands.push(concat(op1, op2)); //Concatenate and add back
                            }
                        } else if(op_sym == '|'){
                            for(int j=0;j<op_count+1;j++){
                                    NFA a;
                                    selections.push_back(a);
                                }
                                
                            int tracker = op_count;
                            for(int k = 0; k < op_count + 1; k++) {
                                selections[tracker] = operands.top();
                                operands.pop();
                                tracker--;
                            }
                            operands.push(union_nfa(selections, op_count+1));
                        }   break; 
                }
            }
        }
        return operands.top(); // Return the resulting entity at top of stack
    }


int main(){
    string re;
        cout<<"Enter a regular expression: ";
        cin>>re;   
        cout<<"\nThe required NFA has the transitions: "<<endl;
        NFA req_nfa;
        req_nfa = re_to_nfa(re); // convert to NFA
        req_nfa.display(); // printing resulting NFA

    return 0;
}
// } (a.((a|b)*).c)