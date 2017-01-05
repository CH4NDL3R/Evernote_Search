#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<string>
#include<algorithm>
#include<stdio.h>

using namespace std;

typedef set<string> docset,idset;

typedef map<string, pair<idset,idset> > mymap;

typedef map<string, pair<docset,docset> > iddocmap;

typedef map<string,double> iddatemap;
iddatemap idtodate;


void add(vector<char> v1, mymap &trie, string id, vector<string> vt)
{
    string s1="";
    int a,b,c,d;
    for(a=0;a<v1.size();a++)
    {
        if(((int(v1[a]))>=48 && (int(v1[a]))<=57) || ((int(v1[a]))>=65 && (int(v1[a]))<=90) || ((int(v1[a]))>=97 && (int(v1[a]))<=122) || (int(v1[a])==39))
            s1+=tolower(v1[a]);
        else
        {
            if (s1==" " || s1=="\t")
            {
                s1="";
                continue;
            }
            if (v1[a]==' ' || (v1[a]!=' ' && v1[a+1]==' '))
            {
                if (trie.find(s1)==trie.end())
                {
                    idset i1,i2;
                    i1.insert(id);
                    //cout<<"inserting "<<s1<<endl;
                    trie[s1]=make_pair(i1,i2);
                }
                else
                    trie[s1].first.insert(id);
                s1="";
            }
            else
            {
                if (trie.find(s1)==trie.end())
                {
                    idset i1,i2;
                    i1.insert(id);
                    //cout<<"inserting "<<s1<<endl;
                    trie[s1]=make_pair(i1,i2);
                }
                else
                    trie[s1].first.insert(id);
            }
        }
    }
    for(a=0;a<vt.size();a++)
    {
        if(trie.find(vt[a])==trie.end())
        {
            idset i1,i2;
            i2.insert(id);
            //cout<<"inserting "<<vt[a]<<endl;
            trie[vt[a]]=make_pair(i1,i2);
        }
        else
            trie[vt[a]].second.insert(id);
    }
}

void createbag(vector<char> v1, iddocmap &bag, string id, vector<string> vt)
{
    docset bagwords,bagtag;
    string s1="";
    int a,b,c,d;
    for(a=0;a<v1.size();a++)
    {
        if(((int(v1[a]))>=48 && (int(v1[a]))<=57) || ((int(v1[a]))>=65 && (int(v1[a]))<=90) || ((int(v1[a]))>=97 && (int(v1[a]))<=122) || (int(v1[a])==39))
            s1+=tolower(v1[a]);
        else
        {
            if (s1==" " || s1=="\t")
            {
                s1="";
                continue;
            }

            if (v1[a]==' ' || (v1[a]!=' ' && v1[a+1]==' '))
            {
                bagwords.insert(s1);
                s1="";
            }
            else
                bagwords.insert(s1);
        }
    }
    for(a=0;a<vt.size();a++)
        bagtag.insert(vt[a]);

    bag[id]=make_pair(bagwords,bagtag);
}

void deletefromtrie(mymap &trie, pair<docset,docset> bagofid, string id)
{
    int a,b,c;
    string s1,s2;
    char ch;
    docset:: iterator i1;
    for(i1=bagofid.first.begin();i1!=bagofid.first.end();i1++)
    {
        trie[*i1].first.erase(id);
        if(trie[*i1].first.size()==0 && trie[*i1].second.size()==0)
            trie.erase(*i1);
    }

    for(i1=bagofid.second.begin();i1!=bagofid.second.end();i1++)
    {
        trie[*i1].second.erase(id);
        if(trie[*i1].first.size()==0 && trie[*i1].second.size()==0)
            trie.erase(*i1);
    }

}

void deletefrombag(iddocmap &bag, string id)
{
    bag[id].first.clear();
    bag[id].second.clear();
    bag.erase(id);
}

void addtoset(idset &i1, idset i2)
{
    if(i1.size()==0)
        i1=i2;
    else
    {
        idset i3;
        idset ::iterator pos1;
        for(pos1=i1.begin();pos1!=i1.end();pos1++)
        {
            if(i2.find(*pos1)!=i2.end())
                i3.insert(*pos1);
        }
        i1=i3;
        i3.clear();
    }
}

bool sorttime(const string &s1, const string &s2)
{
    return idtodate[s1]*1.0 < idtodate[s2]*1.0;
}

void searchtrie(vector<string > vs, mymap trie, iddatemap idtodate2)
{
    int a,b,c;
    string s1;
    idset i1,istar, itime;
    idset:: iterator pos1,pos2;
    mymap:: iterator pos;
    for(a=0;a<vs.size();a++)
    {
        s1=vs[a];
        //cout<<"searching for "<<s1<<endl;
        if(s1=="*")
        {
            for(pos=trie.begin();pos!=trie.end();pos++)
            {
                for(pos2=pos->second.first.begin();pos2!=pos->second.first.end();pos2++)
                    istar.insert(*pos2);
            }
            addtoset(i1,istar);
        }

        else if(s1=="tag:*")
        {
            for(pos=trie.begin();pos!=trie.end();pos++)
            {
                for(pos2=pos->second.second.begin();pos2!=pos->second.second.end();pos2++)
                    istar.insert(*pos2);
            }
            addtoset(i1,istar);
        }

        else if(s1.substr(0,4)=="tag:" && s1[s1.length()-1]!='*')
        {
            s1=s1.substr(4);
            if(trie.find(s1)==trie.end())
            {
                //cout<<"NO RESULT"<<endl;
                cout<<endl;
                i1.clear();
                return;
            }

            else
                addtoset(i1,trie[s1].second);

        }

        else if(s1.substr(0,4)!="tag:" && s1[s1.length()-1]=='*')
        {
            s1=s1.substr(0,s1.length()-2);
            for(pos=trie.begin();pos!=trie.end();pos++)
            {
                if(pos->first.find(s1)==string::npos)
                    continue;
                for(pos2=pos->second.first.begin();pos2!=pos->second.first.end();pos2++)
                    istar.insert(*pos2);
            }
            addtoset(i1,istar);

        }

        else if(s1.substr(0,4)=="tag:" && s1[s1.length()-1]=='*')
        {
            s1=s1.substr(0,s1.length()-2);
            for(pos=trie.begin();pos!=trie.end();pos++)
            {
                if(pos->first.find(s1)==string::npos)
                    continue;
                for(pos2=pos->second.second.begin();pos2!=pos->second.second.end();pos2++)
                    istar.insert(*pos2);
            }
            addtoset(i1,istar);
        }

        else if(s1.substr(0,8)=="created:")
        {
            int a=0;
            a+=(int(s1[15])-48);
            a+=(int(s1[14])-48)*10;
            a+=(int(s1[13])-48)*100;
            a+=(int(s1[12])-48)*1000;
            a+=(int(s1[11])-48)*10000;
            a+=(int(s1[10])-48)*100000;
            a+=(int(s1[9])-48)*1000000;
            a+=(int(s1[8])-48)*10000000;

            iddatemap:: iterator pos;

            for(pos=idtodate2.begin();pos!=idtodate2.end();pos++)
            {
                if(pos->second>a)
                    itime.insert(pos->first);
            }
            if(itime.size()==0)
            {
                cout<<endl;
                i1.clear();
                return;
            }
            else
                addtoset(i1,itime);
        }

        else
        {
            if(trie.find(s1)==trie.end())
            {
                //cout<<"NO RESULT"<<endl;
                cout<<endl;
                i1.clear();
                return;
            }

            else
                addtoset(i1,trie[s1].first);
        }
    }

    vector<string> vres;

    for(pos1=i1.begin();pos1!=i1.end();pos1++)
        vres.push_back(*pos1);

    sort(vres.begin(),vres.end(), sorttime);

    for(c=0;c<vres.size();c++)
    {
        if(c==0)
            cout<<vres[c];
        else
            cout<<","<<vres[c];
    }
    cout<<endl;
    istar.clear();
    vres.clear();
    i1.clear();
}

double convertdate(string s1)
{
    double a;
    while(s1[0]!='<')
        s1=s1.substr(1);
    a=0.0;
    a+=(int(s1[18])-48);
    a+=(int(s1[17])-48)*10;
    a+=(int(s1[15])-48)*100;
    a+=(int(s1[14])-48)*1000;
    a+=(int(s1[12])-48)*10000;
    a+=(int(s1[11])-48)*100000;
    a+=(int(s1[10])-48)*1000000;
    a+=(int(s1[9])-48)*10000000;
    a+=(int(s1[20])-48)*0.1;
    a+=(int(s1[21])-48)*0.01;
    a+=(int(s1[23])-48)*0.001;
    a+=(int(s1[24])-48)*0.0001;
    a+=(int(s1[26])-48)*0.00001;
    a+=(int(s1[27])-48)*0.000001;
    //cout<<"********************************"<<endl<<a<<endl;
    return a;
}


int main()
{
    int a,b,c,d,x,y,id;
    char ch1,ch2;
    double f,date;
    string s1,s2,ss;
    vector<char> v1;
    vector<string> vt,vs;

    mymap trie;

    iddocmap bag;

    string idofnote;

    mymap:: iterator pos1;
    idset:: iterator pos2;

    idset i1,i2;

    while(getline(cin,s1))
    {
        if(s1=="CREATE")
        {
            getline(cin,s2);
            getline(cin,s2);
            x=s2.find("<guid>");
            y=s2.find("</guid>");

            s2=s2.substr(x+6 , y-x-6);

            idofnote=s2;

            getline(cin,s2);
            date=convertdate(s2);

            idtodate[idofnote]=date;

            getline(cin,s2);
            while(s2.find("<tag>")!=string::npos)
            {
                x=s2.find("<tag>");
                y=s2.find("</tag>");

                vt.push_back(s2.substr(x+5,y-x-5));
                getline(cin,s2);
            }

            getline(cin,s2);
            while(s2.find("</content>")==string::npos)
            {
                if(s2.find("</content>")!=string::npos)
                    break;

                for(a=0;a<s2.length();a++)
                    v1.push_back(s2[a]);
                v1.push_back(' ');
                getline(cin,s2);
            }
            getline(cin,s2);

            //add to trie
            add(v1,trie,idofnote,vt);

            //creating bag of words====> id mapped to set of strings the bag contains
            createbag(v1,bag,idofnote,vt);
            vt.clear();
            v1.clear();

        }
        else if(s1=="DELETE")
        {
;
            getline(cin,s2);//id

            idofnote=s2;
            deletefromtrie(trie,bag[idofnote],idofnote);
            deletefrombag(bag,idofnote);

            idtodate.erase(idofnote);
        }

        else if(s1=="UPDATE")
        {

            getline(cin,s2);
            getline(cin,s2);
            x=s2.find("<guid>");
            y=s2.find("</guid>");

            s2=s2.substr(x+6 , y-x-6);

            idofnote=s2;

            deletefromtrie(trie,bag[idofnote],idofnote);
            deletefrombag(bag,idofnote);

            idtodate.erase(idofnote);

            getline(cin,s2);

            date=convertdate(s2);

            idtodate[idofnote]=date;

            getline(cin,s2);
            while(s2.find("<tag>")!=string::npos)
            {
                x=s2.find("<tag>");
                y=s2.find("</tag>");

                vt.push_back(s2.substr(x+5,y-x-5));
                getline(cin,s2);
            }

            getline(cin,s2);
            while(s2.find("</content>")==string::npos)
            {
                if(s2.find("</content>")!=string::npos)
                    break;

                for(a=0;a<s2.length();a++)
                    v1.push_back(s2[a]);
                v1.push_back(' ');
                getline(cin,s2);
            }
            getline(cin,s2);

            //add to trie
            add(v1,trie,idofnote,vt);

            //creating bag of words====> id mapped to set of strings the bag contains
            createbag(v1,bag,idofnote,vt);
            vt.clear();
            v1.clear();

        }

        else if(s1=="SEARCH")
        {

            getline(cin,s2);//id

            string s3="";
            s2+=" ";
            for(a=0;a<s2.length();a++)
            {
                if(s2[a]!=' ')
                    s3+=s2[a];
                else
                {
                    if (s3==" " || s3=="\t")
                    {
                        s3="";
                        continue;
                    }

                    vs.push_back(s3);
                    s3="";
                }
            }
            searchtrie(vs,trie,idtodate);
            vs.clear();

        }
    }

}
