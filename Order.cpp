#ifndef orderr
#define orderr

#include "includes.hpp"
int ttt2 = 0;
#include "Train.hpp"
#include "Date.hpp"
#include "User.hpp"
#include "bplustree.hpp"

char sss[3][10] = {"success", "pending", "refunded"};

const int success = 0;
const int pending = 1;
const int refunded = 2;

fstream OrderData;
fstream OrderMapGO;

struct Who {
	char TrainID[35], from[35], to[35];
	TimePoint L, R;
	int Price, Seat;
	pair <int, string> x;
	friend bool operator < (Who a, Who b) {
		return a.x < b.x;
	}
};

string getS(char *s) {
	int l = strlen(s);
	string cxt; cxt.clear();
	for(int i = 0; i < l; ++ i)
	cxt.push_back(s[i]);
	return cxt;
}

struct Order {
    int Sta;
	int UserID, TrainID, UserNum, Num, Cost, cur, StartSta, EndSta;
	friend bool operator == (Order a, Order b) {
	    return (a.Sta == b.Sta) &&
                (a.UserID == b.UserID) &&
                (a.TrainID == b.TrainID) &&
                (a.Num == b.Num) &&
                (a.Cost == b.Cost) &&
                (a.cur == b.cur)
                && (a.StartSta == b.StartSta) &&
                (a.EndSta == b.EndSta);
	}
    Order(int a = 0, int b = 0, int c = 0, int d = 0, int e = 0, int f = 0, int g = 0, int h = 0) : UserID(a), TrainID(b), UserNum(c), 
	Num(d), Cost(e), cur(f), StartSta(g), EndSta(h) {
		Sta = 0;
    }
        
    bool valid() {
        FileOperator.get(SeatSold, BegOfSold[TrainID] + (cur - 1) * R * sizeof(int), R, Seatsold);
        int Min = 1e9;
        for(int i = StartSta; i < EndSta; ++ i)
        Min = min(Min, Seatsold[i]);
        return Min >= Num;
    }

    void doit(int k) {
        FileOperator.get(SeatSold, BegOfSold[TrainID] + (cur - 1) * R * sizeof(int), R, Seatsold);
	    for(int i = StartSta; i < EndSta; ++ i) {
   	      	 Seatsold[i] -= Num * k;
   	    }
        FileOperator.write(SeatSold, BegOfSold[TrainID] + (cur - 1) * R * sizeof(int), R, Seatsold);
	}

    void print() {
        Train tmp[1];
        FileOperator.get(TrainData, BegOfTrain[TrainID], 1, tmp);
        printf("[%s] ", sss[Sta]);
        printf("%s ", IdGetter.Tra[TrainID]);
        printf("%s ", IdGetter.Sta[tmp[0].Station[StartSta]]);
        TimePoint Beg = tmp[0].LDate;
        Beg.Hour = tmp[0].StartTime.Hour;
        Beg.Minute = tmp[0].StartTime.Minute;
        (Beg + (cur - 1) * 1440 + tmp[0].Time[StartSta - 1] + tmp[0].StopTime[StartSta]).print();
        printf(" -> ");
        printf("%s ", IdGetter.Sta[tmp[0].Station[EndSta]]);
        (Beg + (cur - 1) * 1440 + tmp[0].Time[EndSta - 1]).print();
        printf(" %d %d\n", tmp[0].Price[EndSta - 1] - tmp[0].Price[StartSta - 1], Num);
        return;
    }
    void print2() {
        Train tmp[1];
        FileOperator.get(TrainData, BegOfTrain[TrainID], 1, tmp);
        cerr << '[' <<sss[Sta] <<']' << ' ';
        cerr << IdGetter.Tra[TrainID] << ' ';
        cerr << IdGetter.Sta[tmp[0].Station[StartSta]] << ' ';

        cerr << " -> ";
        cerr << IdGetter.Sta[tmp[0].Station[EndSta]] <<' ';
        cerr << ' ' << tmp[0].Price[EndSta - 1] - tmp[0].Price[StartSta - 1] <<' ' <<  Num << endl;
        return;
    }
};


sjtu::BPtree<pii, Order> OrderMap("order.bpt", "order.data");
//map < pii, Order > OrderMap;

//struct MyOrderMap {
//   	pii a[1]; Order b[1];
//
//   	MyOrderMap() {
//   		OrderMap.clear();
//   		OrderMapGO.open("OrderMapGO", ios::in|ios::out|ios::binary);
//   		int End = FileOperator.getend(OrderMapGO);
//		for(int i = 0; i < End; i += sizeof(Order)) {
//   			FileOperator.get(OrderMapGO, i, 1, a);
//   			i += sizeof(pii);
//   			FileOperator.get(OrderMapGO, i, 1, b);
//   			OrderMap[a[0]] = b[0];
////   			b[0].print2();
//		}
//   		OrderMapGO.close();
//	}
//
//	~MyOrderMap() {
//		FileOperator.NewFile("OrderMapGO");
//		OrderMapGO.open("OrderMapGO", ios::in|ios::out|ios::binary);
//		int pos = 0;
//		for(auto V : OrderMap) {
//			a[0] = V.first;
//			b[0] = V.second;
//			FileOperator.write(OrderMapGO, pos, 1, a);
//			pos += sizeof(pii);
//			FileOperator.write(OrderMapGO, pos, 1, b);
//			pos += sizeof(Order);
//		}
//		OrderMapGO.close();
//	}
//}OdM;

fstream FirGO, TailGO, cntGO, whoGO, nxtGO;

class Orderoperator {
public:
    char opt[N * 10];
    int fir[N], tail[N], cnt;
	pii who[SF];
    int nxt[SF];
    
    void init() {
    	memset(fir, 0, sizeof(fir));
    	memset(tail, 0, sizeof(tail));
    	cnt = 0;
    	memset(who, 0, sizeof(who));
    	memset(nxt, 0, sizeof(nxt));
    	OrderMap.clear();
	}

    Orderoperator() {
    	int tmp[2] = {0};
    	cntGO.open("cntGO", ios::binary|ios::in|ios::out);
    	FirGO.open("FirGO", ios::binary|ios::in|ios::out);
    	TailGO.open("TailGO", ios::binary|ios::in|ios::out);
    	whoGO.open("whoGO", ios::binary|ios::in|ios::out);
    	nxtGO.open("nxtGO", ios::binary|ios::in|ios::out);
		if(FileOperator.getend(cntGO) < 2) {
			memset(fir, 0, sizeof(fir));
			memset(tail, 0, sizeof(tail));
			cnt = 0;
			memset(who, 0, sizeof(who));
			memset(nxt, 0, sizeof(nxt));
			return;
		}
		FileOperator.get(cntGO, 0, 2, tmp);
		cnt = tmp[0];
		FileOperator.get(FirGO, 0, tmp[1], fir + 1);
		FileOperator.get(TailGO, 0, tmp[1], tail + 1);
		FileOperator.get(whoGO, 0, cnt, who + 1);
		FileOperator.get(nxtGO, 0, cnt, nxt + 1);
    	cntGO.close(); FirGO.close(); TailGO.close(); whoGO.close();
    }
    
    ~Orderoperator() {
    	int tmp[2] = {0};
    	FileOperator.NewFile("cntGO");
    	FileOperator.NewFile("FirGO");
    	FileOperator.NewFile("TailGO");
    	FileOperator.NewFile("whoGO");
    	FileOperator.NewFile("nxtGO");
     	cntGO.open("cntGO", ios::binary|ios::in|ios::out);
    	FirGO.open("FirGO", ios::binary|ios::in|ios::out);
    	TailGO.open("TailGO", ios::binary|ios::in|ios::out);
    	whoGO.open("whoGO", ios::binary|ios::in|ios::out);
    	nxtGO.open("nxtGO", ios::binary|ios::in|ios::out);
		tmp[0] = cnt; tmp[1] = N - 1;
		FileOperator.write(cntGO, 0, 2, tmp);
		FileOperator.write(FirGO, 0, N - 1, fir + 1);
		FileOperator.write(TailGO, 0, N - 1, tail + 1);
		FileOperator.write(whoGO, 0, cnt, who + 1);
		FileOperator.write(nxtGO, 0, cnt, nxt + 1);
    	cntGO.close(); FirGO.close(); TailGO.close(); whoGO.close();
		return;   	
    }
    
	std :: string cmd;
	int tot = 0, pos = 0;

	void Nxt() {
		for(int i = 0; i <= tot; ++ i) opt[i] = 0;
		tot = 0;
		while(cmd[pos] == ' ') ++ pos;
		while(pos < (int)cmd.size() && cmd[pos] != ' ') {
			opt[tot ++] = cmd[pos];
			++ pos;
		}
		opt[tot] = '\0';
	}
    
    long long BuyTicket() {
        using std :: string;
        using std :: cin;
        int UserID = 0, TrainID = 0, Start = 0, End = 0, Num = 0;
		bool ok = 0;
        pos = tot = 0;
        TimePoint cur;
		getline(cin, cmd); if((int)cmd.back() == 13) cmd.pop_back();
		while(pos < (int) cmd.size()) {
			Nxt();
			if(!strcmp(opt, "-u")) {
				Nxt();
				UserID = IdGetter.getUser(opt);
			} else if(!strcmp(opt, "-i")) {
				Nxt();
				TrainID = IdGetter.getTrain(opt);
			} else if(!strcmp(opt, "-d")) {
				Nxt();
				cur.Month = (opt[0] - '0') * 10 + opt[1] - '0';
				cur.Date = (opt[3] - '0') * 10 + opt[4] - '0';
			} else if(!strcmp(opt, "-n")) {
				Nxt();
				Num = getnum(opt);
			} else if(!strcmp(opt, "-f")) {
				Nxt();
				Start = IdGetter.QueryStation(opt);
			} else if(!strcmp(opt, "-t")) {
				Nxt();
				End = IdGetter.QueryStation(opt);
			} else if(!strcmp(opt, "-q")) {
				Nxt();
				ok = !strcmp(opt, "true");
			}
		}
		
		if(!UserID || !TrainID || !Start || !End ||
		!Us[UserID].Online
//		|| !Num
		) return -1;
		Train tmp[1];
		FileOperator.get(TrainData, BegOfTrain[TrainID], 1, tmp);
		int _a = 0, _b = 0;
		for(int i = 1; i <= tmp[0].StationNum; ++ i) {
			if(tmp[0].Station[i] == Start) {
				_a = i;
			}
			if(tmp[0].Station[i] == End) {
				_b = i;
			}
		}
		
		if(!tmp[0].Released || tmp[0].Del) return -1;
		
		if(_a > _b || !_a || !_b) return -1;
		TimePoint StartWhat = tmp[0].LDate;
		StartWhat.Hour = tmp[0].StartTime.Hour;
		StartWhat.Minute = tmp[0].StartTime.Minute;
		StartWhat = StartWhat + tmp[0].Time[_a - 1] + tmp[0].StopTime[_a]; 
		StartWhat.Hour = 0, StartWhat.Minute = 0;
		if(cur < StartWhat) return -1;
		int Days = tmp[0].RDate - tmp[0].LDate + 1;
		if((StartWhat + (Days - 1) * 1440) < cur) return -1;
		int where = (cur - StartWhat) + 1;
		int curr = where;
		int StartSta = _a, EndSta = _b, Cost = tmp[0].Price[_b - 1] - tmp[0].Price[_a - 1], 
		UserNum = Us[UserID].OrderNum + 1;
		Order NewOne(UserID, TrainID, UserNum, Num, Cost, curr, StartSta, EndSta);
		bool it = NewOne.valid();
		if(Num > tmp[0].SeatNum) return -1;
		
		if(!it && !ok) return - 1;
		if(it) {
			Us[UserID].OrderNum ++;
			NewOne.Sta = success;
                sjtu::BPtree<pii, Order>::value_type a(make_pair(UserID, Us[UserID].OrderNum),NewOne);
                OrderMap.insert(a);
		//	OrderMap[make_pair(UserID, Us[UserID].OrderNum)] = NewOne;
			NewOne.doit(1);
			return 1LL * NewOne.Cost * NewOne.Num;
		} else {
			Us[UserID].OrderNum ++;
			NewOne.Sta = pending;
                sjtu::BPtree<pii, Order>::value_type a(make_pair(UserID, Us[UserID].OrderNum),NewOne);
              OrderMap.insert(a);
			//OrderMap[make_pair(UserID, Us[UserID].OrderNum)] = NewOne;
			who[++ cnt] = make_pair(UserID, Us[UserID].OrderNum);
			if(!fir[TrainID]) {
				fir[TrainID] = tail[TrainID] = cnt;
			} else {
				nxt[tail[TrainID]] = cnt;
				tail[TrainID] = cnt;
			}
			return -2;
		}
    }

    int QueryOrder() {
        scanf("%s", opt);
        scanf("%s", opt);
        cout<<opt<<"******"<<endl;
        int UserID = IdGetter.getUser(opt);
        if(!UserID || !Us[UserID].Online) {
            return -1;
        }
        printf("%d\n", Us[UserID].OrderNum);
        for(int i = Us[UserID].OrderNum; i >= 1; -- i) {
            std::vector< sjtu::BPtree<pii, Order>::value_type> ans=OrderMap.find(make_pair(UserID, i));
            Order it =ans[0].second;
			it.print(); 
		}
        return 0;
    }

    int RefundTicket() {
        using std :: cin;
        using std :: string;
        pos = 0; int cnt = 1, UserID;
        getline(cin, cmd); if((int)cmd.back() == 13) cmd.pop_back();
      // cout<< cmd<<"    *******     in refund "<<endl;

       	while(pos < cmd.size()) {
       		Nxt();
 			if(!strcmp(opt, "-u")) {
				Nxt();
				UserID = IdGetter.getUser(opt);
			} else {
				Nxt();
				cnt = getnum(opt);
			}
		} 
        if(!UserID || !Us[UserID].Online) {
            return -1;
        }
        if(cnt > Us[UserID].OrderNum) {
            return -1;
        }
        cnt = Us[UserID].OrderNum - cnt + 1;
        std::vector< sjtu::BPtree<pii, Order>::value_type> ans=OrderMap.find(make_pair(UserID, cnt));
        Order now=ans[0].second;
        if(now.Sta == refunded) {
            return -1;
        }
        if(now.Sta == pending) {
            int lst = 0;
            for(int i = fir[now.TrainID]; i; i = nxt[i]) {
                if(who[i] == make_pair(UserID, cnt)) {
                    if(lst > 0) 
                    nxt[lst] = nxt[i];
                    else fir[now.TrainID] = nxt[i];
                    if(tail[now.TrainID] == i) {
                        tail[now.TrainID] = lst;
                    }
                }
                lst = i;
            }
            now.Sta = refunded;



                sjtu::BPtree<pii, Order>::value_type a(make_pair(UserID, cnt),now);;
                OrderMap.update(a);



           // OrderMap[make_pair(UserID, cnt)] = now;
            return 0;
        }
        else {
            now.doit(-1);
            int lst = 0;
            for(int i = fir[now.TrainID]; i; i = nxt[i]) {

                std::vector< sjtu::BPtree<pii, Order>::value_type> ans=OrderMap.find(who[i]);
                Order  NowElement =ans[0].second;



              //  Order NowElement = OrderMap[who[i]];
                if(NowElement.valid()) {
                    NowElement.doit(1);
                    NowElement.Sta = success;


                        sjtu::BPtree<pii, Order>::value_type a(who[i],NowElement);
                        OrderMap.update(a);


                   // OrderMap[who[i]] = NowElement;
                    if(lst > 0) {
                    	nxt[lst] = nxt[i];
					} else fir[now.TrainID] = nxt[i];
					if(tail[now.TrainID] == i)
					tail[now.TrainID] = lst;
                } else // kiao
                lst = i;
            }
        }
        now.Sta = refunded;



            sjtu::BPtree<pii, Order>::value_type a(make_pair(UserID, cnt),now);
            OrderMap.update(a);


       // OrderMap[make_pair(UserID, cnt)] = now;
        return 0;
    }
    
    Who ans[N * 5];
    
    void QueryTicket() {
    	using std :: cin;
		getline(cin, cmd); if((int)cmd.back() == 13) cmd.pop_back();
		int Start = 0, End = 0;
		TimePoint cur;
		bool ok = 0;
		pos = 0;
		while(pos < (int) cmd.size()) {
			Nxt();
			if(!strcmp(opt, "-s")) {
				Nxt();
				Start = IdGetter.QueryStation(opt);
			} else if(!strcmp(opt, "-t")) {
				Nxt();
				End = IdGetter.QueryStation(opt);
			} else if(!strcmp(opt, "-d")) {
				Nxt();
				cur.Month = (opt[0] - '0') * 10 + opt[1] - '0';
				cur.Date = (opt[3] - '0') * 10 + opt[4] - '0';
			} else if(!strcmp(opt, "-p")) {
				Nxt();
				ok = strcmp(opt, "time");
			}
		}
		
		if(!Start || !End) {
			puts("0");
			return;
		}
		
		map < pii, int> :: iterator Lpointer, Rpointer, it;
		
		Lpointer = Station_To_Train.lower_bound(make_pair(Start, 0));
		
		if(Lpointer == Station_To_Train.end() || (Lpointer -> first).first != Start) {
			puts("0");
			return;
		}
		
		Rpointer = Station_To_Train.upper_bound(make_pair(Start, 1000000000));
		int tot = 0;
		Train tmp[1];
		for(it = Lpointer; it != Rpointer; ++ it) {
			int TrainID = (it -> first).second, where = (it -> second);
			if(Station_To_Train.count(make_pair(End, TrainID))) {
				int where2 = Station_To_Train[make_pair(End, TrainID)];
				if(where2 > where) {
					FileOperator.get(TrainData, BegOfTrain[TrainID], 1, tmp);
					TimePoint StartWhat = tmp[0].LDate, xxx;
					StartWhat.Hour = tmp[0].StartTime.Hour;
					StartWhat.Minute = tmp[0].StartTime.Minute;
					xxx = StartWhat;
					StartWhat = StartWhat + tmp[0].Time[where - 1] + tmp[0].StopTime[where]; 
					StartWhat.Hour = 0, StartWhat.Minute = 0;
					if(cur < StartWhat) continue;
					int Days = (tmp[0].RDate - tmp[0].LDate) + 1;
					if((StartWhat + (Days - 1) * 1440) < cur) continue;
					
					++ tot; int itDay = (cur - StartWhat) + 1;
					strcpy(ans[tot].from, IdGetter.Sta[Start]);
					strcpy(ans[tot].to, IdGetter.Sta[End]);
					ans[tot].L = xxx + tmp[0].Time[where - 1] + (itDay - 1) *1440 + tmp[0].StopTime[where];
					ans[tot].R = xxx + tmp[0].Time[where2 - 1] + (itDay - 1) * 1440;
					ans[tot].Price = tmp[0].Price[where2 - 1] - tmp[0].Price[where - 1];
					strcpy(ans[tot].TrainID, IdGetter.Tra[TrainID]);
           			FileOperator.get(SeatSold, BegOfSold[TrainID] + (itDay - 1) * R * sizeof(int), R, Seatsold);
           			int Min = 1e9;
           			for(int i = where; i < where2; ++ i)
           			Min = min(Min, Seatsold[i]);
           			ans[tot].Seat = Min;
           			string now = getS(IdGetter.Tra[TrainID]);
           			ans[tot].x 
           			= make_pair((ok) ? (ans[tot].Price) : (ans[tot].R.calc() - ans[tot].L.calc()), now);
				}
			}
		}
		sort(ans + 1, ans + tot + 1);
		printf("%d\n", tot);
		for(int i = 1; i <= tot; ++ i) {
			printf("%s %s ", ans[i].TrainID, ans[i].from);
			ans[i].L.print(); printf(" -> ");
			printf("%s ", ans[i].to);
			ans[i].R.print();
			printf(" %d %d\n", ans[i].Price, ans[i].Seat);
		}
	}
	
	#define mp make_pair
	
	pair < pair <int, int> , pair <TimePoint, int> >  Min[N];
	
	void calc(Who &it, int TrainID, int S, int T, TimePoint cur) {
		Train tmp[1];
		strcpy(it.from, IdGetter.Sta[S]);
		strcpy(it.to, IdGetter.Sta[T]);
		strcpy(it.TrainID, IdGetter.Tra[TrainID]);
		FileOperator.get(TrainData, BegOfTrain[TrainID], 1, tmp);
		TimePoint StartWhat = tmp[0].LDate, xxx;
		StartWhat.Hour = tmp[0].StartTime.Hour;
		StartWhat.Minute = tmp[0].StartTime.Minute;
		xxx = StartWhat;
		int where = 0, where2 = 0;
		for(int i = 1; i <= tmp[0].StationNum; ++ i) {
			if(tmp[0].Station[i] == S) where = i;
			if(tmp[0].Station[i] == T) where2 = i;
		}
		StartWhat = StartWhat + tmp[0].Time[where - 1] + tmp[0].StopTime[where]; 
		StartWhat.Hour = 0, StartWhat.Minute = 0;
		int itDays = (cur - StartWhat) + 1;
		
		it.L = (xxx + tmp[0].Time[where - 1] + tmp[0].StopTime[where] + (itDays - 1) * 1440);
		it.R = (xxx + tmp[0].Time[where2 - 1] + (itDays - 1) * 1440);
		it.Price = tmp[0].Price[where2 - 1] - tmp[0].Price[where - 1];
		it.Seat = 1e9;
		FileOperator.get(SeatSold, BegOfSold[TrainID] + sizeof(int) * R * (itDays - 1), R, Seatsold);
		for(int i = where; i < where2; ++ i) 
		it.Seat = min(it.Seat, Seatsold[i]);
		return;
	}
	
	void QueryTransfer() {
    	using std :: cin;
		getline(cin, cmd); if((int)cmd.back() == 13) cmd.pop_back();
		int Start = 0, End = 0;
		TimePoint cur;
		bool ok = 0;
		pos = 0;
		while(pos < (int) cmd.size()) {
			Nxt();
			if(!strcmp(opt, "-s")) {
				Nxt();
				Start = IdGetter.QueryStation(opt);
			} else if(!strcmp(opt, "-t")) {
				Nxt();
				End = IdGetter.QueryStation(opt);
			} else if(!strcmp(opt, "-d")) {
				Nxt();
				cur.Month = (opt[0] - '0') * 10 + opt[1] - '0';
				cur.Date = (opt[3] - '0') * 10 + opt[4] - '0';
			} else if(!strcmp(opt, "-p")) {
				Nxt();
				ok = strcmp(opt, "time");
			}
		}
		
		if(!Start || !End) {
			puts("0");
			return;
		}
		
		Who L, R; pair <int, int> ans = mp(2e9, 2e9);
		
		map < pii, int> :: iterator Lpointer, Rpointer, it;
		Lpointer = Station_To_Train.lower_bound(make_pair(Start, 0));
		if(Lpointer == Station_To_Train.end() || (Lpointer -> first).first != Start) {
			puts("0");
			return;
		}
		
		TimePoint INF; INF.Month = 12; INF.Date = 1;
		
		for(int i = 1; i <= IdGetter.cntAll; ++ i) {
			Min[i] = make_pair(mp(2e9, 2e9), mp(INF, 2e9));
		}
		
		int NowDay = 0, RTrainID = 0, who = 0, LTrainID = 0, Mid = 0;
		TimePoint Time;
		Rpointer = Station_To_Train.upper_bound(make_pair(Start, 1000000000));
		int tot = 0;
		Train tmp[2];
		for(it = Lpointer; it != Rpointer; ++ it) {
			int TrainID = (it -> first).second, where = (it -> second);
			FileOperator.get(TrainData, BegOfTrain[TrainID], 1, tmp);
			TimePoint StartWhat = tmp[0].LDate, xxx;
			StartWhat.Hour = tmp[0].StartTime.Hour;
			StartWhat.Minute = tmp[0].StartTime.Minute;
			xxx = StartWhat;
			StartWhat = StartWhat + tmp[0].Time[where - 1] + tmp[0].StopTime[where]; 
			StartWhat.Hour = 0, StartWhat.Minute = 0;
			if(cur < StartWhat) continue;
			int Days = (tmp[0].RDate - tmp[0].LDate) + 1;
			if((StartWhat + (Days - 1) * 1440) < cur) continue;
			int itDays = (cur - StartWhat) + 1;
			xxx = xxx + (itDays - 1) * 1440;
			
			
			for(int i = where + 1; i <= tmp[0].StationNum; ++ i) {
				int it = tmp[0].Station[i];
				int a = - (xxx + tmp[0].Time[where - 1] + tmp[0].StopTime[where]).calc(), 
				b = tmp[0].Price[i - 1] - tmp[0].Price[where - 1], 
				c = tmp[0].Time[i - 1] - tmp[0].Time[where - 1] - tmp[0].StopTime[where];
				if(!ok)
				Min[it] = (make_pair( mp(a, c), 
				mp(xxx + tmp[0].Time[i - 1], TrainID)));
				else 
				Min[it] = (make_pair(mp(b, c), 
				mp(xxx + tmp[0].Time[i - 1], TrainID)));
	
			}
			map < pii, int> :: iterator _Lpointer, _Rpointer, _it;
			_Lpointer = Station_To_Train.lower_bound(make_pair(End, 0));
			if(_Lpointer == Station_To_Train.end() || (_Lpointer -> first).first != End) {
				puts("0");
				return;
			}
		
			_Rpointer = Station_To_Train.upper_bound(make_pair(End, 1000000000));
				
			int ho = TrainID;
				
			for(_it = _Lpointer; _it != _Rpointer; ++ _it) {
				int TrainID = (_it -> first).second, where = (_it -> second);
				if(TrainID == ho) continue;
				FileOperator.get(TrainData, BegOfTrain[TrainID], 1, tmp + 1);
				TimePoint StartWhat = tmp[1].LDate, xxx;
				StartWhat.Hour = tmp[1].StartTime.Hour;
				StartWhat.Minute = tmp[1].StartTime.Minute;
				xxx = StartWhat;
				int Day = (tmp[1].RDate - tmp[1].LDate) + 1;
				for(int j = 1; j <= where - 1; ++ j) {
					int it = tmp[1].Station[j];
					if(Min[it].first.first > 5e8 || Min[it].second.second != ho) continue;
					TimePoint yyy = xxx + tmp[1].Time[j - 1] + tmp[1].StopTime[j], zzz;
					if((Min[it].second.first).calc() > yyy.calc() + (Day - 1) * 1440) {
						continue;
					} else {
						int now = (Min[it].second.first.calc() - yyy.calc() + 1439) / 1440;
						now = max(now, 0);
						int a = 
						(tmp[1].Price[where - 1] - tmp[1].Price[j - 1]), b = 
						(xxx + tmp[1].Time[where - 1]).calc() + now * 1440;
						if(!ok) swap(a, b);
						pair <int, int> val = mp(Min[it].first.first + a, Min[it].first.second);
						if(val < ans) {
							ans = val; NowDay = now; who = j; RTrainID = TrainID;
							LTrainID = Min[it].second.second;
							Mid = tmp[1].Station[who];
							Time = yyy + now * 1440;
							Time.Hour = 0; Time.Minute = 0;
						}
					}
				}
			} 				
		}
		
		if(ans.first > 5e8) {
			puts("0");
			return;
		}
		
		calc(L, LTrainID, Start, Mid, cur);
		calc(R, RTrainID, Mid, End, Time);
		
		printf("%s %s ", L.TrainID, L.from);
		L.L.print(); printf(" -> ");
		printf("%s ", L.to);
		L.R.print();
		printf(" %d %d\n", L.Price, L.Seat);
		
		printf("%s %s ", R.TrainID, R.from);
		R.L.print(); printf(" -> ");
		printf("%s ", R.to);
		R.R.print();
		printf(" %d %d\n", R.Price, R.Seat);
		return;
	}

}OrderOperator;

int main() {
	freopen("data.txt", "r", stdin);
	freopen("data.out", "w", stdout);
	TrainData.open("TrainData", ios::binary|ios::in|ios::out);
    SeatSold.open("SeatSold", ios::binary|ios::in|ios::out);
    OrderData.open("OrderData", ios::binary|ios::in|ios::out);
	if(FileOperator.getend(TrainData) < 0)
	TrainData.close(), 
	FileOperator.NewFile("TrainData"), 
	TrainData.open("TrainData", ios::binary|ios::in|ios::out);
    if(FileOperator.getend(SeatSold) < 0)
	SeatSold.close(),
	FileOperator.NewFile("SeatSold"), 
	SeatSold.open("SeatSold", ios::binary|ios::in|ios::out);
    if(FileOperator.getend(OrderData) < 0) 
	OrderData.close(),
	FileOperator.NewFile("OrderData"), 
	OrderData.open("OrderData", ios::binary|ios::in|ios::out);
    char opt[105];
    int tot = 0, ok = 0;
	ttt2 = 0;
	while(scanf("%s", opt) == 1) {
		++ ttt2;
		if(!strcmp(opt, "exit")) {
			puts("bye");
			TrainData.close();
			SeatSold.close();
			OrderData.close();
			UserOperator.OffLine();
			return 0;
		}
		if(!strcmp(opt, "clean")) {
			TrainData.close();
			SeatSold.close();
			OrderData.close(); 
			FileOperator.NewFile("TrainData");
   			FileOperator.NewFile("SeatSold");
    		FileOperator.NewFile("OrderData");
			TrainData.open("TrainData", ios::binary|ios::in|ios::out);
    		SeatSold.open("SeatSold", ios::binary|ios::in|ios::out);
    		OrderData.open("OrderData", ios::binary|ios::in|ios::out);
    		UserOperator.init();
    		TrainOperator.init();
    		IdGetter.init();
    		OrderOperator.init();
    		throw;
    		puts("0");
    		continue;
		}
		if(!strcmp(opt, "add_user")) {
			int cur = UserOperator.AddUser();
			printf("%d\n", cur);
		} else if(!strcmp(opt, "login")) {
			printf("%d\n", UserOperator.Login());
		} else if(!strcmp(opt, "logout")) {
			printf("%d\n", UserOperator.Logout());
		} else if(!strcmp(opt, "query_profile")) {
			int cur = UserOperator.QueryProfile();
			if(cur == -1) printf("%d\n", cur);
		} else if(!strcmp(opt, "modify_profile")) {
			int cur = UserOperator.ModifyProfile();
			if(cur == -1) printf("%d\n", cur);
		} else if(!strcmp(opt, "buy_ticket")) {
            long long cur = OrderOperator.BuyTicket();
            if(cur == -2) puts("queue");
            else printf("%lld\n", cur);
        } else if(!strcmp(opt, "query_order")) {
            int cur = OrderOperator.QueryOrder();
            if(cur == -1) {
                puts("-1");
            }
        } else if(!strcmp(opt, "refund_ticket")) {
            int cur = OrderOperator.RefundTicket();
            if(cur == -1) {
                puts("-1");
            } else puts("0");
        } else if(!strcmp(opt, "add_train")) {
			printf("%d\n", TrainOperator.AddTrain());
		} else if(!strcmp(opt, "query_train")) {
			int cur = TrainOperator.QueryTrain();
			if(cur == -1) puts("-1");
		} else if(!strcmp(opt, "release_train")) {
			printf("%d\n", TrainOperator.ReleaseTrain());
		} else if(!strcmp(opt, "delete_train")) {
			printf("%d\n", TrainOperator.DeleteTrain());
		} else if(!strcmp(opt, "query_ticket")) {
			OrderOperator.QueryTicket();
		} else if(!strcmp(opt, "query_transfer")) {
			OrderOperator.QueryTransfer();
		}
	}
}
#endif

