//We will comment the change only
#include <iostream>
#include <vector>
#include <string>
#include<fstream>

using namespace std;

vector<string> split(const string& target, const string& delimiter){

    vector<string> toBeReturned;
    size_t nextDelimiterIndex = target.find(delimiter,0);

    if(nextDelimiterIndex < target.size())
        toBeReturned.push_back(target.substr(0, nextDelimiterIndex));

    while(nextDelimiterIndex<target.size()-1 &&
    nextDelimiterIndex < target.find(delimiter,nextDelimiterIndex+1)){

        toBeReturned.push_back(target.substr(nextDelimiterIndex+1,
        target.find(delimiter,nextDelimiterIndex+1) - nextDelimiterIndex - 1));

        nextDelimiterIndex = target.find(delimiter,nextDelimiterIndex+1);
    };

    return toBeReturned;
};

vector<vector<vector<string>>> racersDataExtractor(const vector<string>
&myFileLastLines){

    vector<vector<vector<string>>> toBeReturned;
    for(size_t j=0;j<myFileLastLines.size()/3;++j){//we changed 3 to \
                                        myFileLastLines.size()/3 (sensors no.)
        //appending a new point
        toBeReturned.push_back(vector<vector<string>>());
        for(size_t i=0;i<3;++i)
            toBeReturned[j].push_back(split(myFileLastLines[i+3*j],","));

        if(j>0){
            vector<string> temp;
            for(size_t k1 = 0; k1<3;++k1){
                for(size_t k2 = 0; k2<3;++k2){
                    if(toBeReturned[j][k1][1]==toBeReturned[j-1][k2][1]){
                        temp = toBeReturned[j][k1];
                        toBeReturned[j][k1] = toBeReturned[j][k2];
                        toBeReturned[j][k2] = temp;

                    };

                };
            };
        };
    };

    return toBeReturned;
};

int main(){

    fstream myData("racers' data.dat", ios::in);
    if(!myData){
        cout<<"I couldn't find \"racers' data.dat\" in the current"
        " directory.\n";
        system("pause");
        exit(1);
    };

    string myFileString;
    char next;
    while(myData.get(next)) myFileString+=next;

    string startTime = myFileString.substr(0, myFileString.find("\n"));

    string dataLines = myFileString.substr(myFileString.find("\n")+1);

    vector<string> dataLinesVect = split(dataLines, "\n");

    vector<vector<vector<string>>> racersData =
        racersDataExtractor(dataLinesVect);

    cout<<"Enter the racer's ID:\t";
    string racerID;cin>>racerID;

    int racerNumber =-1;
    for(size_t i =0; i<3;++i)
        if(racersData[0][i][1] == racerID) racerNumber = i;

    if (racerNumber<0){
        cout<<"\nInvalid ID.\n";
        system("pause");
        exit(1);
    };


    //We replced startTimeVec, secondTimeVec ... in the previous version with\
    the following vector (structured data)
    vector<vector<string>> sensorsTimes;
    for(size_t sensorNo = 0; sensorNo < racersData.size(); ++ sensorNo)
        sensorsTimes.push_back(split(racersData[sensorNo][racerNumber][2]," "));

    //We replced startTimeMinutes, secondTimeMinutes ... in the previous \
    version with the following vector (structured data)
    vector<double> sensorsTimesMinutes;
    for(size_t sensorNo = 0; sensorNo < racersData.size(); ++ sensorNo)
        sensorsTimesMinutes.push_back(stod(sensorsTimes[sensorNo][0])*60+
        stod(sensorsTimes[sensorNo][1])+
        stod(sensorsTimes[sensorNo][2])/60);

    //Getting the sensors markers. The user should provide with each sensor \
    position in Mile
    vector<double> sensorsPlacesMile;
    double tempLocation;
    for(size_t sensorNo = 0; sensorNo < racersData.size(); ++ sensorNo){
         cout<<"\nEnter the place where sensor"<<sensorNo+1
         <<" locates (Mile)\nEx: for sensor1 enter 0, for sensor2 enter 7 for"
         " sensor3 enter 13.1 :\t";
        cin>>tempLocation;
        sensorsPlacesMile.push_back(tempLocation);//appending sensor of number\
                                                    (sensorNo +1) location
    };


    //Displaying the results
    cout<< "the racer's overall finish place is "<<racersData[2][racerNumber][2];

    for(size_t sensorNo = 0; sensorNo < racersData.size()-1; ++ sensorNo)
     cout<<"\nrace split times in minutes/mile for split"<<sensorNo+1<<" is "<<
     (sensorsTimesMinutes[sensorNo+1] - sensorsTimesMinutes[sensorNo])/
     (sensorsPlacesMile[sensorNo+1]-sensorsPlacesMile[sensorNo]);

    cout<<"\nthe overall race time (minutes) is "<<
    sensorsTimesMinutes[sensorsTimesMinutes.size()-1] - sensorsTimesMinutes[0]<<
    "\nand overall race pace (minutes/mile) is "<<
    (sensorsTimesMinutes[sensorsTimesMinutes.size()-1] - sensorsTimesMinutes[0])/
     (sensorsPlacesMile[sensorsPlacesMile.size()-1]-sensorsPlacesMile[0])
    <<"." ;

    return 0;

};

