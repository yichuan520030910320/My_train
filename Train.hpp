#ifndef Trainn
#define Trainn
#include "includes.hpp"
#include "Date.hpp"
#include "FileOperator.hpp"
#include "Hash.hpp"
#include <bits/stdc++.h>

fstream TrainData;
fstream SeatSold;
int Seatsold[R];
int BegOfSold[N], BegOfTrain[N];
map < pair <int, int>, int> Station_To_Train;

fstream StationGO;

struct MyStation {
	map < pii, int> Map;
   	pii a[1]; int b[1];
    	
   	MyStation() {
   		StationGO.open("StationGO", ios::in|ios::out|ios::binary);
   		int End = FileOperator.getend(StationGO);
   		for(int i = 0; i < End; i += sizeof(int)) {
   			FileOperator.get(StationGO, i, 1, a);
   			i += sizeof(pii);
   			FileOperator.get(StationGO, i, 1, b);
   			Map[a[0]] = b[0];
		}
   		swap(Map, Station_To_Train);
   		StationGO.close();
	}
   	
	~MyStation() {
		swap(Map, Station_To_Train);
		FileOperator.NewFile("StationGO");
		StationGO.open("StationGO", ios::in|ios::out|ios::binary);
		int pos = 0;
		for(auto V : Map) {
			a[0] = V.first;
			b[0] = V.second;
			FileOperator.write(StationGO, pos, 1, a);
			pos += sizeof(pii);
			FileOperator.write(StationGO, pos, 1, b);
			pos += sizeof(int);
//			cerr << a[0].first <<' ' << a[0].second <<" " << b[0] << endl;
		}
		StationGO.close();
	}
}MyS;

struct Train {
	int Price[R] = {0}, Time[R] = {0}, Station[R] = {0}, StopTime[R] = {0};
	int StationNum, SeatNum, Released, Del, fir;
	TimePoint LDate, RDate, StartTime;
	char Type;
	
	Train() {
		StationNum = SeatNum = Released = Type = Del = fir = 0;
		memset(Price, 0, sizeof(Price));
		memset(Time, 0, sizeof(Time));
		memset(Station, 0, sizeof(Station));
		memset(StopTime, 0, sizeof(StopTime));
	}
	
	void print(TimePoint cur, int ID) {
		char opt[N + 5];
		int whi = (cur - LDate) + 1;
		cur.Hour = StartTime.Hour;
		cur.Minute = StartTime.Minute;
		FileOperator.get(SeatSold, BegOfSold[ID] + (whi - 1) * sizeof(int) * R, R, Seatsold);
		for(int i = 1; i <= StationNum; ++ i) {
			if(i == 1) {
				IdGetter.getStation(Station[i], opt);
				printf("%s xx-xx xx:xx -> ", opt);
				cur.print();
				printf(" %d %d\n", Price[i - 1], Seatsold[i]);
			}
			else if(i < StationNum) {
				IdGetter.getStation(Station[i], opt);
				printf("%s ", opt);
				(cur + Time[i - 1]).print();
				printf(" -> ");
				(cur + Time[i - 1] + StopTime[i]).print();
				printf(" %d %d\n", Price[i - 1], Seatsold[i]);
			} else {
				IdGetter.getStation(Station[i], opt);
				printf("%s ", opt);
				(cur + Time[i - 1]).print();
				printf(" -> ");
				printf("xx-xx xx:xx");
				printf(" %d x\n", Price[i - 1]);					
			}
		}
		return;
	}	
};

fstream TrainNumGO, BegOfTrainGO, BegOfSoldGO;

class Trainoperator {

// 存储在TrainData这个文件里

public:
	const int Size = (int)sizeof(Train);
	Train tmp[2];
	
	char opt[N * 20], TrainID[105], Name[105];
	
	int TrainNum = 0, Zero[R * R];
	
	void init() {
		TrainNum = 0;Station_To_Train.clear();
		memset(BegOfSold, 0, sizeof(BegOfSold));
		memset(BegOfTrain, 0, sizeof(BegOfTrain));
	}
	
    Trainoperator() {
    	int tmp[1] = {0};
    	TrainNumGO.open("TrainNumGO", ios::binary|ios::in|ios::out);
		BegOfSoldGO.open("BegOfSoldGO", ios::binary|ios::in|ios::out);
		BegOfTrainGO.open("BegOfTrainGO", ios::binary|ios::in|ios::out);
		if(FileOperator.getend(TrainNumGO) == -1) {
			return;
		}
		FileOperator.get(TrainNumGO, 0, 1, tmp);
		TrainNum = tmp[0];
		FileOperator.get(BegOfSoldGO, 0, TrainNum,BegOfSold + 1);
 		FileOperator.get(BegOfTrainGO, 0, TrainNum, BegOfTrain + 1);
//		cerr <<"A" << endl;
//		for(int i = 1; i <= TrainNum; ++ i) cerr << BegOfSold[i] <<' ';
//		cerr << endl;
//		for(int i = 1; i <= TrainNum; ++ i) cerr << BegOfTrain[i] <<' ';
//		cerr << endl; 
//		cerr << "A" << endl;
		
    	TrainNumGO.close();
		BegOfSoldGO.close();
		BegOfTrainGO.close();
    }
    
    ~Trainoperator() {
    	FileOperator.NewFile("TrainNumGO");
    	FileOperator.NewFile("BegOfSoldGO");
    	FileOperator.NewFile("BegOfTrainGO");
    	int tmp[1] = {0};
    	TrainNumGO.open("TrainNumGO", ios::binary|ios::in|ios::out);
		BegOfSoldGO.open("BegOfSoldGO", ios::binary|ios::in|ios::out);
		BegOfTrainGO.open("BegOfTrainGO", ios::binary|ios::in|ios::out);
		tmp[0] = TrainNum;
		FileOperator.write(TrainNumGO, 0, 1, tmp);
		FileOperator.write(BegOfSoldGO, 0, TrainNum,BegOfSold + 1);
 		FileOperator.write(BegOfTrainGO, 0, TrainNum, BegOfTrain + 1);
//		cerr <<"B" << endl;
//		for(int i = 1; i <= TrainNum; ++ i) cerr << BegOfSold[i] <<' ';
//		cerr << endl;
//		for(int i = 1; i <= TrainNum; ++ i) cerr << BegOfTrain[i] <<' ';
//		cerr << endl; 
//		cerr << "B" << endl;
    	TrainNumGO.close();
		BegOfSoldGO.close();
		BegOfTrainGO.close();
    }  
    
	
	int AddTrain() {
		using std :: string;
		using std :: cin;
		Train NewOne;
		string cmd;
		int ans = 0;
		for(int i = 1; i <= 10; ++ i) {
			scanf("%s", opt);
			if(!strcmp(opt, "-i")) {
				scanf("%s", TrainID);
				if(IdGetter.getTrain(TrainID)) {
					ans = -1;
				}
			} else if(!strcmp(opt, "-n")) scanf("%d", &NewOne.StationNum);
			else if(!strcmp(opt, "-m")) scanf("%d", &NewOne.SeatNum);
			else if(!strcmp(opt, "-s")) {
				if(!NewOne.StationNum) throw;
				scanf("%s", opt); int pos = 0, l = strlen(opt), lst = 0;
				for(int i = 1; i <= NewOne.StationNum; ++ i) {
					int tot = 0;
					while(pos < l && opt[pos] != '|') {
						Name[tot ++] = opt[pos ++];
					}
					++ pos;
					Name[tot] = '\0';
					int now = IdGetter.HashStation(Name);
					NewOne.Station[i] = now;
				}			
			} else if(!strcmp(opt, "-p")) {
				if(!NewOne.StationNum) throw;
				scanf("%s", opt); int pos = 0, l = strlen(opt), lst = 0;
				for(int i = 1; i < NewOne.StationNum; ++ i) {
					int tot = 0;
					while(pos < l && opt[pos] != '|') {
						Name[tot ++] = opt[pos ++];
					}
					++ pos;
					Name[tot] = '\0';
					NewOne.Price[i] = (lst += getnum(Name));
				}			
			} else if(!strcmp(opt, "-x")) {
				scanf("%s", opt);
				NewOne.StartTime.Hour = 10 * (opt[0] - '0') + opt[1] - '0';
				NewOne.StartTime.Minute = 10 * (opt[3] - '0') + opt[4] - '0';
				NewOne.StartTime.Month = NewOne.StartTime.Date = 1;				
			} else if(!strcmp(opt, "-t")) {
				if(!NewOne.StationNum) throw;
				scanf("%s", opt); int pos = 0, l = strlen(opt), lst = 0;
				for(int i = 1; i <= NewOne.StationNum; ++ i) {
					int tot = 0;
					while(pos < l && opt[pos] != '|') {
						Name[tot ++ ] = opt[pos ++];
					}
					++ pos;
					Name[tot] = '\0';
					NewOne.Time[i] = (getnum(Name));
				}				
			} else if(!strcmp(opt, "-o")) {
				if(!NewOne.StationNum) throw;
				scanf("%s", opt); int pos = 0, l = strlen(opt);
				if(NewOne.StationNum == 2) continue; 
				for(int i = 2; i < NewOne.StationNum; ++ i) {
					int tot = 0;
					while(pos < l && opt[pos] != '|') {
						Name[tot ++] = opt[pos ++];
					}
					++ pos;
					Name[tot] = '\0';
					NewOne.StopTime[i] = getnum(Name);
				}				
			} else if(!strcmp(opt, "-d")) {
				scanf("%s", opt);
				NewOne.LDate.Month = 10 * (opt[0] - '0') + opt[1] - '0';
				NewOne.LDate.Date = 10 * (opt[3] - '0') + opt[4] - '0';
				NewOne.RDate.Month = 10 * (opt[6] - '0') + (opt[7] - '0');
				NewOne.RDate.Date = 10 * (opt[9] - '0') + (opt[10] - '0');			
			} else if(!strcmp(opt, "-y")) {
				scanf("%s", opt);
				NewOne.Type = opt[0];				
			}
			
		}
		if(ans == -1 || !NewOne.StartTime.valid() || !NewOne.LDate.valid() || !NewOne.RDate.valid()
		|| NewOne.RDate < NewOne.LDate || !NewOne.SeatNum) {
			return -1;
		}
//		if(ans == -1 || !NewOne.SeatNum) return -1;
		int pos = FileOperator.getend(TrainData);
		for(int i = 1; i < NewOne.StationNum; ++ i) {
			NewOne.Time[i] += NewOne.StopTime[i];
			NewOne.Time[i + 1] += NewOne.Time[i];
		}
		
		
		tmp[0] = NewOne;
		int ID = ++ TrainNum, T = (NewOne.RDate - NewOne.LDate) + 1;
		IdGetter.addTrain(TrainID, ID);
		BegOfTrain[ID] = pos;
		FileOperator.write(TrainData, pos, 1, tmp);
        pos = FileOperator.getend(SeatSold);
        BegOfSold[ID] = pos;
        for(int i = 0; i < T * R; ++ i) Zero[i] = NewOne.SeatNum;
        FileOperator.write(SeatSold, pos, T * R, Zero);
		return 0;
	}

	int QueryTrain() {
		TimePoint cur;
		int ID = 0;
		scanf("%s", opt);
		if(!strcmp(opt, "-d")) {
			scanf("%s", opt);
			cur.Month = 10 * (opt[0] - '0') + opt[1] - '0';
			cur.Date = 10 * (opt[3] - '0') + opt[4] - '0';
		} else {
			scanf("%s", TrainID);
			ID = IdGetter.getTrain(TrainID);
		}
		scanf("%s", opt);
		if(!strcmp(opt, "-d")) {
			scanf("%s", opt);
			cur.Month = 10 * (opt[0] - '0') + opt[1] - '0';
			cur.Date = 10 * (opt[3] - '0') + opt[4] - '0';
		} else {
			scanf("%s", TrainID);
			ID = IdGetter.getTrain(TrainID);
		}
		if(!ID) {
			return -1;
		}
		FileOperator.get(TrainData, BegOfTrain[ID], 1, tmp);
		
		if(tmp[0].Del) return -1;
		
		if(tmp[0].RDate < cur || cur < tmp[0].LDate) return -1;
		
		printf("%s %c\n", TrainID, tmp[0].Type);
		tmp[0].print(cur, ID);
		return 0;
	}
	
	int ReleaseTrain() {
		scanf("%s", opt);
		scanf("%s", TrainID);
		int ID = IdGetter.getTrain(TrainID);
		if(!ID) return -1;
		FileOperator.get(TrainData, BegOfTrain[ID], 1, tmp);
		if(tmp[0].Del || tmp[0].Released) return -1;
		tmp[0].Released = 1;
		FileOperator.write(TrainData, BegOfTrain[ID], 1, tmp);
		for(int i = 1; i <= tmp[0].StationNum; ++ i) {
			Station_To_Train[make_pair(tmp[0].Station[i], ID)] = i;
		}
		return 0;
	}
	
	int DeleteTrain() {
		scanf("%s", opt);
		scanf("%s", TrainID);
		int ID = IdGetter.getTrain(TrainID);
		if(!ID) return -1;
		FileOperator.get(TrainData, BegOfTrain[ID], 1, tmp);
		if(tmp[0].Del || tmp[0].Released) return -1;
		tmp[0].Del = 1;
		FileOperator.write(TrainData, BegOfTrain[ID], 1, tmp);
		return 0;
	}
}TrainOperator;
#endif
