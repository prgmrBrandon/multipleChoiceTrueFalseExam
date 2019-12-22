#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;


class Question{

private:

              string question;

              int value;

public:
        Question(){};
        Question(string theQuestion, int pointValue){
             question=theQuestion;
              value=pointValue;
         }

         string getQuestion(){
             return question;
         }

         int getValue(){
           return value;
         }

         virtual string printOptions(){return "";};
         virtual string getAnswer() {return "";};

};

class QuestionMC : public Question{
    private:
        string answer;
        string options[6];
        
    public:
        QuestionMC(string theQuestion, int pointValue, string theAnswer) : Question(theQuestion, pointValue){
                             answer = theAnswer;
                             for (int i=0; i<6; i++){
                                 options[i] = "";
                             }
         }

         void addOption(string anOption){
             for (int i=0; i<6; i++) {
                 if (options[i]=="") {
                     options[i] = anOption;
                     break;
                 }
             }
         }

         string printOptions(){
             string str = "";
             for (int i=0; i<6; i++) {
                 if (options[i] != "") {
                    str = str + (char)(65+i) + ". " + options[i] + "\n";
                 }
                 else {
                     break;
                 }                             
             }
             
             return str;
         }

         string getAnswer() {
             return answer;
         }
};


class QuestionTF : public Question {

    private:
        string answer;

    public:
        QuestionTF(string theQuestion, int pointValue, string theAnswer) : Question(theQuestion, pointValue) {
            answer = theAnswer;
         }

         string printOptions() {
             return "True or False";
         }

         string getAnswer() {
             return answer;
         }
};


class Examination{
    private:
        vector<Question*> questions;

    public:

    void LoadQuestions(ifstream &fin){
        questions.clear();
        int numberofquestions;
        string questiontype;
        string questiontext;
        string correctanswer;

        int pointvalue;

        fin >> numberofquestions;

        string extra;                            
        //  char ex;

        for (int i=0; i<numberofquestions; i++){
            fin >> questiontype;
            if (questiontype == "TF"){
                fin >> pointvalue;
                fin >> extra;
                getline(fin, questiontext);                                 
                fin >> correctanswer;
                QuestionTF *questiontf = new QuestionTF(questiontext, pointvalue, correctanswer);
                    questions.push_back(questiontf);                                                      
            }
            else if (questiontype == "MC"){
                fin >> pointvalue;
                fin.ignore();
                getline(fin, questiontext);
                int numberofoptions;
                fin >> numberofoptions;
                string *answers;
                answers = new string[numberofoptions];
                fin.ignore();
                for (int i=0; i<numberofoptions; i++){
                    getline(fin, answers[i]);
                }

                fin >> correctanswer;
                QuestionMC *questionmc = new QuestionMC(questiontext, pointvalue, correctanswer);
                for (int i=0; i<numberofoptions; i++){
                    questionmc->addOption(answers[i]);
                }
                
                questions.push_back(questionmc);
            }
        }
    }

    int GetNumberOfQuestions(){
        return questions.size();
    }

    string getCorrectAnswer(int index){
        return questions[index]->getAnswer();
    }
    
    void DisplayIthQuestion(int index){
        cout << "Question Number" << (index+1) << ": ";
        cout << questions[index]->getQuestion() << endl;
        cout << questions[index]->printOptions() << endl;
    }

    int getPointsValue(int i){
        return questions[i]->getValue();
    }

    void DisplayQuestions(){
        for (unsigned int i=0; i<questions.size(); i++){
            cout << "Question Number" << (i+1) << ": ";
            cout << questions[i]->getQuestion() << endl;
            cout << questions[i]->printOptions() << endl;
            cout << "Correct Answer: " << questions[i]->getAnswer() << endl << endl;
        }
    }
};

 class Student{
    private:
        int pointsAvailable;
        int pointsScored;
              
    public:
        Student(){
            pointsAvailable = 0; 
            pointsScored = 0;
        }

        void SetPointsScored(int point){
            pointsScored = point;
        }

        int GetPointsScored(){
            return pointsScored;
       }

       void SetPointsAvailable(int point){
           pointsAvailable = point;
       }

       int GetPointsAvailable(){
           return pointsAvailable;
       }
};

char DisplayMenu();

string ConvertToupper(string str); 



int main(){
    Examination exam;
    char option;
    //int totalpoints = 0;
    //int totalAvailablePoints = 0;
    option = DisplayMenu();
    Student student;
        while (option != 'Q'){
            switch (option){
                case 'L':  {
                    string filename;
                    cout << "Enter filename: ";
                    cin >> filename;
                    ifstream fin;
                    fin.open(filename.c_str());
                        while (!fin){
                            cout << "Error: Input file does not exists. Try again." << endl;
                            cout << "Enter filename: ";
                            cin >> filename;
                            fin.open(filename.c_str());
                        }
                        
                    exam.LoadQuestions(fin);
                    fin.close();
                }

                break;

                case  'T': {
								    string answer;                 

                    for (int i=0; i<exam.GetNumberOfQuestions(); i++){
                        cout << endl;
                        exam.DisplayIthQuestion(i);
                        cout << "Enter correct answer: ";
                        cin >> answer;
                        answer = ConvertToupper(answer);
                        string correct = ConvertToupper(exam.getCorrectAnswer(i));
                        cout << "The correct answer is: " << correct << endl;
                        if (answer == correct){
                            student.SetPointsScored (student.GetPointsScored() + exam.getPointsValue(i));
                            cout << "Good job!" << endl;
                        }
                        else {
                            cout << "Better luck next time..." << endl;
                        }

                        student.SetPointsAvailable (student.GetPointsAvailable() + exam.getPointsValue(i));

								    }                                                    
                }

                break;

                case  'S': {
                    cout << "Total points available: " << student.GetPointsAvailable() << endl;
                    cout << "Total points scored: " << student.GetPointsScored() << endl;
                    cout << "Percentage of points: " << ((double)student.GetPointsScored()/(double)student.GetPointsAvailable()) * 100.0 << endl;
                }
                
                break;
            }
            
            option = DisplayMenu();
        }
        
        return 0;
}

string ConvertToupper(string str){
    for (unsigned int i=0; i<str.length(); i++){
        str[i] = toupper(str[i]);
    }

    return str;
}

char DisplayMenu(){
    char option;
    while (true){
        cout << endl << "\tL - Load an Exam" << endl;
        cout << "\tT -Take an exam Exam" << endl;
        cout << "\tS - Show exam results" << endl;
        cout << "\tQ - Quit" << endl;
        cout << "Enter an option: ";
        cin >> option;
        option = toupper (option);
        
        if (option == 'L' || option == 'T' || option == 'S' ||option == 'Q')
            return option;
        else
            cout << "ERROR: Input a valid option." << endl;
		}
}
