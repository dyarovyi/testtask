#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <map>
#include <vector>


// --------------------------------------------------------------------------------
const std::string months[12] = 
{
    "January", "February", "March", 
    "April", "May", "June", 
    "July", "August", "September", 
    "October", "November", "December"
};


// --------------------------------------------------------------------------------
struct Date
{
    int year;
    int month;
    int day;

    std::string datestr;

    Date(int y, int m, int d): year(y), month(m), day(y), datestr("") {}
    Date(std::string d): year(0), month(0), day(0), datestr(d) {}
};


// --------------------------------------------------------------------------------
struct Data
{
    std::string name;
    std::string email;
    std::string department;
    std::string position;
    std::string project;
    std::string task;
    Date date;
    double hours;

    Data(std::string n, std::string e, std::string d, std::string p, std::string j, std::string t, Date a, double h):
        name(n),
        email(e),
        department(d),
        position(p),
        project(j),
        task(t),
        date(a),
        hours(h)
        {}
};


// --------------------------------------------------------------------------------
struct Total
{
    std::string name;
    Date date;
    double hours;

    Total(std::string n, Date d, double h): name(n), date(d), hours(h) {}

    void setHours(double newHours)
    {
        hours = newHours;
    }
};


// --------------------------------------------------------------------------------
void readData(std::vector<Data> &logList, std::ifstream &input)
{
    std::string name;
    std::string email;
    std::string department;
    std::string position;
    std::string project;
    std::string task;
    Date *date;
    double hours;

    std::string line;
    std::string word;

    while (getline(input, line))
    {
        std::stringstream str(line);
        std::vector<std::string> row;
        std::vector<int> rowdate;
 
        while (getline(str, word, ';'))
        {
            row.push_back(word);
        }

        std::stringstream strdate(row[6]);
        while (getline(strdate, word, '-'))
        {
            rowdate.push_back(atoi(word.c_str()));
        }

        name = row[0];
        email = row[1];
        department = row[2];
        position = row[3];
        project = row[4];
        task = row[5];
        date = new Date(rowdate[0], rowdate[1], rowdate[2]);
        hours = atof(row[7].c_str());
            
        Data *element = new Data(name, email, department, position, project, task, *date, hours);
        logList.push_back(*element);
    }
}


// --------------------------------------------------------------------------------
void calculate(std::vector<Data> &logList, std::vector<Total> &totalList)
{
    for (int i = 0; i < logList.size(); i++)
    {
        bool flag = true;
        std::stringstream year;
        year << logList[i].date.year;

        std::string datestr = months[logList[i].date.month - 1] + " ";
        datestr += year.str();

        for (int j = 0; j < totalList.size(); j++)
        {
            if (totalList[j].name == logList[i].name && totalList[j].date.datestr == datestr)
            {
                totalList[j].setHours(totalList[j].hours + logList[i].hours); 
                flag = false;
            }
        }
        if (flag)
        {
            Date *date = new Date(datestr);
            Total *element = new Total(logList[i].name, *date, logList[i].hours);
            totalList.push_back(*element);
        }
    }

    for (int i = 0; i < totalList.size(); i++)
    {
        std::cout << totalList[i].name << " " << totalList[i].date.datestr << " " << totalList[i].hours << std::endl;
    }
    std::cout << "after calc" << std::endl;
}


// --------------------------------------------------------------------------------
void writeData(std::vector<Total> &totlaList, std::ofstream &output)
{
    for (int n = 0; n < totlaList.size(); n++)
    {
        Total i = totlaList[n];
        output 
            << i.name << ";"
            << i.date.datestr << ";"
            << i.hours << "\n"; 
    }
}


// --------------------------------------------------------------------------------
void writeDataFull(std::vector<Data> &logList, std::ofstream &output)
{
    for (int n = 0; n < logList.size(); n++)
    {
        Data i = logList[n];
        output 
            << i.name << ";"
            << i.email << ";"
            << i.department << ";"
            << i.position << ";"
            << i.project << ";"
            << i.task << ";"
            << i.date.year << "-" << i.date.month << "-" << i.date.day << ";"
            << i.hours 
            << "\n"; 
    }
}


// --------------------------------------------------------------------------------
int main()
{
    std::ifstream input;
    std::ofstream output;
    std::vector<Data> logList;
    std::vector<Total> totalList;

    input.open("input.csv");
    readData(logList, input);
    input.close();

    calculate(logList, totalList);

    output.open("output.csv");
    writeData(totalList, output);
    output.close();

    return 0;
}