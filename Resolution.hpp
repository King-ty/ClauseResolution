#ifndef RESOLUTION_CLASSES
#define RESOLUTION_CLASSES

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <cctype>
using namespace std;

/************
 * 参数结构体，用于描述原子句的参数，包括谓词名、常量、变量三种类型
 * */
struct Parameter
{
	static const int PREDICATE = 0, //谓词
		CONSTANT = 1,				//常量
		VARIABLE = 2;				//变量
	int type;						//参数类型
	string name;					//参数名字符串

	/************
	 * 空参构造，此时参数类型不为三种类型之一
	 * */
	Parameter() : type(-1) {}

	/************
	 * 2参构造
	 * @param type		参数类型
	 * @param name		参数名
	 * */
	Parameter(int type, const string &name) : type(type), name(name) {}

	/************
	 * 重载<，用于map
	 * @param temp	用于比较的另一个参数对象常引用
	 * */
	bool operator<(const Parameter &temp) const { return name < temp.name; }
};

/************
 * 原子句结构体，用于描述一个原子句
 * */
struct Atomic_clause
{
	vector<Parameter> paras; //原子句的参数，其中参数[0]固定为谓词
	bool _true;				 //原子句是否为真

	/************
	 * 空参构造，默认为真
	 * */
	Atomic_clause() : _true(1) {}

	/************
	 * 展示原子句，TODO：改为<<
	 * @param _debug	是否为debug模式（可输出更多信息供调试）
	 * */
	void show(bool _debug = 0) const;

	/************
	 * 清空原子句
	 * */
	void clear() { paras.clear(), _true = 1; }
};

/************
 * 子句结构体，用于描述一个子句，一般由多个以或相连的原子句构成
 * */
struct Clause
{
	static const int NONE = -1; //无father或mother
	vector<Atomic_clause> sub;	//原子句数组
	int mother;					//归结生成该子句的双亲之一
	int father;					//归结生成该子句的另一个双亲
	bool valid;					//该子句是否可用

	/************
	 * 带默认参数构造函数
	 * */
	Clause(int mother = Clause::NONE, int father = Clause::NONE, bool valid = 1) : mother(mother), father(father), valid(valid) {}

	/************
	 * 重载==
	 * */
	bool operator==(const Clause &temp) const;

	/************
	 * 展示子句，TODO：改为<<
	 * @param _debug	是否为debug模式（可输出更多信息供调试）
	 * */
	void show(bool _debug = 0) const;
};

/************
 * 归结类，用于描述一次归结
 * */
class Resolution
{
private:
	static const int HAS_ANS = 1, //归结得到答案
		HAS_PARADOX = -1,		  //归结得到悖论
		NO_ANS = 0;				  //无法完成归结或还未完成归结

	vector<Clause> clauses; //子句集
	Clause goal_c;			//目标（结论）子句
	int ansno;				//归结答案状态
	int _step;				//用于输出归结步骤时记录步数

	/************
	 * 将字符串转为新子句
	 * @param s		输入要转为子句的字符串
	 * @param op	默认为0，将转出的新子句加入子句集；op==1时表示此时处理的是结论串不加入子句集
	 * */
	bool addClause(string s, int op = 0);

	/************
	 * 判断子句{@code c1}是否为子句{@code c2}的祖先子句，即子句{@code c2}是否由{@code c1}归结而来
	 * @param c1	子句c1在子句集中的下标
	 * @param c2	子句c2在子句集中的下标
	 * */
	bool isAncestor(int c1, int c2);

	/************
	 * 用一个子句对其他子句进行一次归结
	 * @param pos	用来归结其他子句的子句在子句集中的位置
	 * @return		返回ansno
	 * */
	int oneResolution(int pos);

	/************
	 * 判断两个原子句是否满足合一条件
	 * @param a1	原子句1
	 * @param a2	原子句2
	 * */
	bool canMerge(const Atomic_clause &a1, const Atomic_clause &a2);

	/************
	 * 将两个子句进行置换合一
	 * @param father	用于置换合一的第一个子句在子句集中下标
	 * @param mother	用于置换合一的第二个子句在子句集中下标
	 * @param _del		置换合一时合掉的原子句在第二个子句中的下标
	 * @return		返回ansno
	 * */
	int mergeClause(int father, int mother, int _del);

	/************
	 * 用于递归输出归结步骤
	 * @param c	当前要输出的子句
	 * */
	void _outSteps(const Clause &c);

public:
	/************
	 * 空参构造，用于初始化类内变量，输出提示并按照提示输入所有子句和结论
	 * */
	Resolution();

	/************
	 * 归结搜索函数，用于实现归结算法
	 * @return	返回ansno
	 * */
	int resolutionSearch();

	/************
	 * 先进行判断和初始化，在调用_outStep函数递归输出归结步骤
	 * @return	能否输出归结步骤，若ansno==NO_ANS则不能输出
	 * */
	bool outSteps();
};

void Atomic_clause::show(bool _debug) const
{
	for (int i = 0; i < paras.size(); ++i)
	{
		cout << paras[i].name;
		if (_debug)
			cout << "_" << paras[i].type;
		if (i == 0)
			cout << "(";
		else if (i != paras.size() - 1)
			cout << ",";
		else
			cout << ")";
	}
}

bool Clause::operator==(const Clause &temp) const
{
	if (sub.size() != temp.sub.size())
		return 0;
	for (int i = 0; i < sub.size(); ++i)
	{
		if (sub[i]._true != temp.sub[i]._true || sub[i].paras.size() != temp.sub[i].paras.size())
			return 0;
		for (int j = 0; j < sub[i].paras.size(); ++j)
		{
			if (sub[i].paras[j].name != temp.sub[i].paras[j].name)
				return 0;
		}
	}
	return 1;
}

void Clause::show(bool _debug) const
{
	for (int i = 0; i < sub.size(); ++i)
	{
		if (i)
			cout << "|";
		if (!sub[i]._true)
		{
			cout << "!";
		}
		sub[i].show(_debug);
	}
	if (_debug)
		cout << endl;
}

bool Resolution::addClause(string s, int op)
{
	string temp_s;
	Atomic_clause temp_a;
	Clause temp_c;
	bool bracket = 0,
		 new_str = 1;
	int para_type = Parameter::PREDICATE;
	for (int i = 0; i < s.length(); ++i)
	{
		if (s[i] == '(')
		{
			if (bracket)
				return false;
			bracket = 1;
			new_str = 1;
			temp_a.paras.push_back(Parameter(para_type, temp_s));
			para_type = Parameter::PREDICATE - 1;
		}
		else if (s[i] == ')')
		{
			if (!bracket)
				return false;
			bracket = 0;
			new_str = 1;
			temp_a.paras.push_back(Parameter(para_type, temp_s));
			temp_c.sub.push_back(temp_a);
			temp_a.clear();
		}
		else if (s[i] == '|')
		{
			para_type = Parameter::PREDICATE;
		}
		else if (s[i] == ',')
		{
			new_str = 1;
			temp_a.paras.push_back(Parameter(para_type, temp_s));
		}
		else
		{
			if (new_str)
			{
				if (s[i] == ' ')
					continue;
				if (s[i] == '!' || s[i] == '~')
				{
					if (para_type != Parameter::PREDICATE)
						return false;
					temp_a._true = 0;
					continue;
				}
				new_str = 0;
				if (para_type != Parameter::PREDICATE)
					if (isupper(s[i]))
						para_type = Parameter::CONSTANT;
					else
						para_type = Parameter::VARIABLE;
				temp_s.clear();
			}
			temp_s += s[i];
		}
	}
	if (op == 0)
		clauses.push_back(temp_c);
	else
		goal_c = temp_c;
	// debug
	// temp_c.show(1);
	return true;
}

Resolution::Resolution() : _step(0)
{
	ansno = Resolution::NO_ANS;
	string temp_s;
	cout << "请输入所有子句('|'代表或，'!'或'~'代表非)，输入完成后最后一行输入'#'表示输入结束\n";
	cout << "Example : \n"
		 << "Kill(A,A)|Kill(B,A)|Kill(C,A)\n"
		 << "~Kill(x1,A)|Hate(x1,A)\n"
		 << "~Hate(A,x2)|~Hate(C,x2)\n"
		 << "Hate(A,A)\n"
		 << "Hate(A,C)\n"
		 << "Richer(x3,A)|Hate(B,x3)\n"
		 << "~Hate(A,x4)|Hate(B,x4)\n"
		 << "~Hate(x5,A)|~Hate(x5,B)|~Hate(x5,C)\n"
		 << "~Kill(x6,A)|~Richer(x6,A)\n"
		 << "#\n"
		 << "(可直接复制示例)\n";
	for (;;)
	{
		cin >> temp_s;
		if (temp_s == "#")
			break;
		if (!addClause(temp_s))
		{
			// cerr << "wrong clause, please input again\n";
			cerr << "子句输入有误，清重新输入！\n";
			// continue;
		}
	}
	cout << "请输入一个结论子句\n";
	cout << "Example : \n"
		 << "Kill(A,A)\n"
		 << "(可直接复制示例)\n";
	cin >> temp_s;
	while (!addClause(temp_s, 1))
	{
		// cerr << "wrong clause, please input again\n";
		cerr << "子句输入有误，清重新输入！\n";
	}
}

bool Resolution::isAncestor(int c1, int c2)
{
	if (clauses[c2].father == Clause::NONE || clauses[c2].mother == Clause::NONE)
		return 0;
	if (clauses[c2].father == c1 || clauses[c2].mother == c1)
		return 1;
	return isAncestor(c1, clauses[c2].father) || isAncestor(c1, clauses[c2].mother);
}

bool Resolution::canMerge(const Atomic_clause &a1, const Atomic_clause &a2)
{
	if (a1.paras[0].name == a2.paras[0].name &&
		a1.paras.size() == a2.paras.size())
	{
		for (int i = 1; i < a1.paras.size(); ++i)
		{
			if (a1.paras[i].type == Parameter::VARIABLE || a2.paras[i].type == Parameter::VARIABLE)
				continue;
			if (a1.paras[i].name != a2.paras[i].name)
				return 0;
		}
		return 1;
	}
	return 0;
}

int Resolution::mergeClause(int father, int mother, int _del) //
{
	Clause new_c;
	map<Parameter, Parameter> replace_map;
	for (int i = 1; i < clauses[father].sub[0].paras.size(); ++i)
	{
		if (clauses[mother].sub[_del].paras[i].type == Parameter::VARIABLE)
			replace_map[clauses[mother].sub[_del].paras[i]] = clauses[father].sub[0].paras[i];
	}
	new_c.sub = clauses[mother].sub;
	new_c.father = father;
	new_c.mother = mother;
	new_c.valid = 1;
	new_c.sub.erase(new_c.sub.begin() + _del);

	for (int i = 0; i < new_c.sub.size(); ++i)
	{
		for (int j = 1; j < new_c.sub[i].paras.size(); ++j)
		{
			if (new_c.sub[i].paras[j].type == Parameter::VARIABLE &&
				replace_map.find(new_c.sub[i].paras[j]) != replace_map.end())
			{
				new_c.sub[i].paras[j] = replace_map[new_c.sub[i].paras[j]];
			}
		}
	}
	if (new_c.sub.size() == 0)
		return (ansno = Resolution::HAS_PARADOX);
	clauses.push_back(new_c);
	if (new_c == goal_c)
		return (ansno = Resolution::HAS_ANS);
	return NO_ANS;
}

int Resolution::oneResolution(int pos)
{
	for (int i = 0; i < clauses.size(); ++i)
	{
		if (clauses[i].valid && !isAncestor(i, pos))
		{
			for (int j = 0; j < clauses[i].sub.size(); ++j)
			{
				if (canMerge(clauses[pos].sub[0], clauses[i].sub[j]) &&
					clauses[pos].sub[0]._true != clauses[i].sub[j]._true)
				{
					mergeClause(pos, i, j);
					if (ansno == HAS_PARADOX || ansno == HAS_ANS)
						return ansno;
				}
			}
		}
	}
	return NO_ANS;
}

int Resolution::resolutionSearch()
{
	Clause nw;
	for (int i = 0; i < clauses.size(); ++i)
	{
		// cout << clauses[i].father << ' ' << clauses[i].mother << endl; //debug
		if (clauses[i].sub.size() == 1)
		{
			clauses[i].valid = 0;
			oneResolution(i);
			if (ansno == HAS_PARADOX || ansno == HAS_ANS)
				return ansno;
		}
	}
	return NO_ANS;
}

void Resolution::_outSteps(const Clause &c)
{
	//debug
	// c.show(1);

	if (c.father == Clause::NONE || c.mother == Clause::NONE)
		return;
	_outSteps(clauses[c.father]);
	_outSteps(clauses[c.mother]);
	cout << "Step" << ++_step << "\t: ";
	clauses[c.father].show();
	cout << "  &  ";
	clauses[c.mother].show();
	cout << "  ==>  ";
	c.show();
	cout << endl;
}

bool Resolution::outSteps()
{
	if (ansno != NO_ANS)
	{
		_step = 0;
		_outSteps(clauses.back());
		return 1;
	}
	return 0;
}

// TODO
// ostream &operator<<(ostream &out, const Clause &c)
// {
// 	c.show();
// }

#endif