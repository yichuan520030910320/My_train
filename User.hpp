#ifndef Userr
#define Userr
#include <cstdio>
#include <iostream>
#include <cstring>
#include "FileOperator.hpp"
#include "Hash.hpp"
#include <string>
#include <sstream>
using namespace std;

fstream UsGO, UserNumGO;

struct Dan {
	int Userid, Trainid, Num, Date, S, T;
};

struct UserData {
	char Password[35] = {0}, Mail[35] = {0}, Name[16] = {0}; 
	int priv, Id, Head, Online, OrderNum;
	UserData() {
		priv = Id = Head = Online = OrderNum = 0;
	}
};

UserData Us[10005];

class User_Operator {
public:
	int UserNum;
	char opt[105], UserName[105], ttt[105];
	
	void init() {
		memset(Us, 0, sizeof(Us));
		UserNum = 0;
	}

    User_Operator() {
    	int tmp[1] = {0};
    	UsGO.open("UsGO", ios::binary|ios::in|ios::out);
    	UserNumGO.open("UserNumGO", ios::binary|ios::in|ios::out);
//    	cerr << UserNum <<' ' << FileOperator.getend(UserNumGO) << endl;
		if(FileOperator.getend(UserNumGO) == -1) {
			memset(Us, 0, sizeof(Us));
			return;
		}
		FileOperator.get(UserNumGO, 0, 1, tmp);
		UserNum = tmp[0];
		FileOperator.get(UsGO, 0, UserNum, Us + 1);
    	UsGO.close(); UserNumGO.close();
    }
    
    ~User_Operator() {
    	FileOperator.NewFile("UsGO");
    	FileOperator.NewFile("UserNumGO");
     	UsGO.open("UsGO", ios::binary|ios::in|ios::out);
    	UserNumGO.open("UserNumGO", ios::binary|ios::in|ios::out);
		int tmp[1] = {0};
		tmp[0] = UserNum;
		FileOperator.write(UserNumGO, 0, 1, tmp);
		FileOperator.write(UsGO, 0, UserNum, Us + 1);
		UserNumGO.close();  UsGO.close();
		return;   	
    }    
	
	int AddUser() {	
		using std :: string;
		using std :: cin;
		++ UserNum;
		int curUser = 0;
		UserData tmp;
		for(int i = 1; i <= 6; ++ i) {
			scanf("%s", opt);
			if(!strcmp(opt, "-u")) {
				scanf("%s", UserName);
			} else if(!strcmp(opt, "-p")) {
				scanf("%s", tmp.Password);
			} else if(!strcmp(opt, "-n")) {
				scanf("%s", tmp.Name);
			} else if(!strcmp(opt, "-m")) {
				scanf("%s", tmp.Mail);
			} else if(!strcmp(opt, "-g")) {
				scanf("%d", &tmp.priv);
			} else if(!strcmp(opt, "-c")) {
				scanf("%s", ttt);
			}
		}
		
		if(IdGetter.getUser(UserName)) {
			--UserNum;
			return -1;
		}
		
		if(UserNum > 1) {
			curUser = IdGetter.getUser(ttt);
			if(!curUser || !Us[curUser].Online) {
				-- UserNum;
				return -1;
			}
		}
		
		if(UserNum > 1) {
			if(tmp.priv >= Us[curUser].priv) {
				-- UserNum;
				return -1;
			}
		} else tmp.priv = 10;
		tmp.Id = UserNum;
		Us[UserNum] = tmp;
		IdGetter.addUser(UserName, tmp.Id);
		/*puts("HERE");
		printf("%s %s %s %d %d %d\n",
		tmp.Name, tmp.Mail, tmp.Password, tmp.Id, tmp.Head, tmp.priv);*/
		return 0;
	}

	char Password[105];
	int Login() {
		scanf("%s", opt); if(opt[1] == 'u') scanf("%s", UserName); else scanf("%s", Password);
		scanf("%s", opt); if(opt[1] == 'u') scanf("%s", UserName); else scanf("%s", Password);
		int User = IdGetter.getUser(UserName);
		if(User && !Us[User].Online && !strcmp(Us[User].Password, Password)) {
			Us[User].Online = 1;
			return 0;
		}
		return -1;
	}

	int Logout() {
		scanf("%s", opt);
		scanf("%s", UserName);
		int User = IdGetter.getUser(UserName);
		if(User && Us[User].Online) {
			Us[User].Online = 0;
			return 0;
		}
		return -1;
	}

	int QueryProfile() {
		scanf("%s", opt); if(opt[1] == 'u') scanf("%s", UserName); else scanf("%s", ttt);
		scanf("%s", opt); if(opt[1] == 'u') scanf("%s", UserName); else scanf("%s", ttt);
		int CurUser = IdGetter.getUser(ttt);
		int User = IdGetter.getUser(UserName);
		if(!Us[CurUser].Online || !User || !CurUser) return -1;
		if(CurUser == User || Us[CurUser].priv > Us[User].priv) {
			printf("%s %s %s %d\n", UserName, Us[User].Name, Us[User].Mail, Us[User].priv);
			return 0;
		}
		return -1;
	}

	std :: string cmd;
	int tot = 0, pos = 0;
	
	void nxt() {
		for(int i = 0; i <= tot; ++ i) opt[i] = 0;
		tot = 0;
		while(cmd[pos] == ' ' || cmd[pos] == '\r') ++ pos;
		while(pos < (int)cmd.size() && cmd[pos] != ' ' && cmd[pos] != '\n') {
			opt[tot ++] = cmd[pos]; 
			++ pos;
		}
		opt[tot] = '\0';
	}

	int ModifyProfile() {
		using std :: cin;
		using std :: string;
		int CurUser = 0;
		int User = 0;
		UserData tmp; tmp.priv = -1;
		getline(cin, cmd); if((int)cmd.back() == 13) cmd.pop_back();pos = 0, tot = 0;
		
		int ans = 0;
		
		while(pos < (int) cmd.size()) {
			nxt();
			if(!strcmp(opt, "-p")) {
				nxt();
				strcpy(tmp.Password, opt);
			} else if(!strcmp(opt, "-n")) {
				nxt();
				strcpy(tmp.Name, opt);
			} else if(!strcmp(opt, "-m")) {
				nxt();
				assert(opt[tot] == '\0');
				strcpy(tmp.Mail, opt);
				for(int i = 0; i < tot; ++ i) assert(opt[i] != '\0');
			} else if(!strcmp(opt, "-g")) {
				nxt();
				tmp.priv = getnum(opt); // kiao
			} else if(!strcmp(opt, "-c")) {
				nxt();
				CurUser = IdGetter.getUser(opt);
			} else if(!strcmp(opt, "-u")) {
				nxt();
				User = IdGetter.getUser(opt);
				strcpy(UserName, opt);
			}
		}
		
		if(!CurUser || !User || !Us[CurUser].Online) {
			return -1;
		} 
		
		if(Us[CurUser].priv <= tmp.priv || Us[CurUser].priv < Us[User].priv) return -1;
		if(CurUser != User && Us[CurUser].priv == Us[User].priv) return -1;
		if(ans == -1) return -1;
		if(strlen(tmp.Name) != 0) strcpy(Us[User].Name, tmp.Name);
		if(tmp.priv >= 0) Us[User].priv = tmp.priv;
		if(strlen(tmp.Password) > 0) strcpy(Us[User].Password, tmp.Password); 
		if(strlen(tmp.Mail) > 0) strcpy(Us[User].Mail, tmp.Mail);
		printf("%s %s %s %d\n", UserName, Us[User].Name, Us[User].Mail, Us[User].priv);
		return 0;
	}
	
	void OffLine() {
		for(int i = 1; i <= UserNum; ++ i) {
			Us[i].Online = 0;
		}
	}
}UserOperator;
#endif

