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
 * �����ṹ�壬��������ԭ�Ӿ�Ĳ���������ν������������������������
 * */
struct Parameter
{
	static const int PREDICATE = 0, //ν��
		CONSTANT = 1,				//����
		VARIABLE = 2;				//����
	int type;						//��������
	string name;					//�������ַ���

	/************
	 * �ղι��죬��ʱ�������Ͳ�Ϊ��������֮һ
	 * */
	Parameter() : type(-1) {}

	/************
	 * 2�ι���
	 * @param type		��������
	 * @param name		������
	 * */
	Parameter(int type, const string &name) : type(type), name(name) {}

	/************
	 * ����<������map
	 * @param temp	���ڱȽϵ���һ��������������
	 * */
	bool operator<(const Parameter &temp) const { return name < temp.name; }
};

/************
 * ԭ�Ӿ�ṹ�壬��������һ��ԭ�Ӿ�
 * */
struct Atomic_clause
{
	vector<Parameter> paras; //ԭ�Ӿ�Ĳ��������в���[0]�̶�Ϊν��
	bool _true;				 //ԭ�Ӿ��Ƿ�Ϊ��

	/************
	 * �ղι��죬Ĭ��Ϊ��
	 * */
	Atomic_clause() : _true(1) {}

	/************
	 * չʾԭ�Ӿ䣬TODO����Ϊ<<
	 * @param _debug	�Ƿ�Ϊdebugģʽ�������������Ϣ�����ԣ�
	 * */
	void show(bool _debug = 0) const;

	/************
	 * ���ԭ�Ӿ�
	 * */
	void clear() { paras.clear(), _true = 1; }
};

/************
 * �Ӿ�ṹ�壬��������һ���Ӿ䣬һ���ɶ���Ի�������ԭ�Ӿ乹��
 * */
struct Clause
{
	static const int NONE = -1; //��father��mother
	vector<Atomic_clause> sub;	//ԭ�Ӿ�����
	int mother;					//������ɸ��Ӿ��˫��֮һ
	int father;					//������ɸ��Ӿ����һ��˫��
	bool valid;					//���Ӿ��Ƿ����

	/************
	 * ��Ĭ�ϲ������캯��
	 * */
	Clause(int mother = Clause::NONE, int father = Clause::NONE, bool valid = 1) : mother(mother), father(father), valid(valid) {}

	/************
	 * ����==
	 * */
	bool operator==(const Clause &temp) const;

	/************
	 * չʾ�Ӿ䣬TODO����Ϊ<<
	 * @param _debug	�Ƿ�Ϊdebugģʽ�������������Ϣ�����ԣ�
	 * */
	void show(bool _debug = 0) const;
};

/************
 * ����࣬��������һ�ι��
 * */
class Resolution
{
private:
	static const int HAS_ANS = 1, //���õ���
		HAS_PARADOX = -1,		  //���õ����
		NO_ANS = 0;				  //�޷���ɹ���δ��ɹ��

	vector<Clause> clauses; //�Ӿ伯
	Clause goal_c;			//Ŀ�꣨���ۣ��Ӿ�
	int ansno;				//����״̬
	int _step;				//���������Ჽ��ʱ��¼����

	/************
	 * ���ַ���תΪ���Ӿ�
	 * @param s		����ҪתΪ�Ӿ���ַ���
	 * @param op	Ĭ��Ϊ0����ת�������Ӿ�����Ӿ伯��op==1ʱ��ʾ��ʱ������ǽ��۴��������Ӿ伯
	 * */
	bool addClause(string s, int op = 0);

	/************
	 * �ж��Ӿ�{@code c1}�Ƿ�Ϊ�Ӿ�{@code c2}�������Ӿ䣬���Ӿ�{@code c2}�Ƿ���{@code c1}������
	 * @param c1	�Ӿ�c1���Ӿ伯�е��±�
	 * @param c2	�Ӿ�c2���Ӿ伯�е��±�
	 * */
	bool isAncestor(int c1, int c2);

	/************
	 * ��һ���Ӿ�������Ӿ����һ�ι��
	 * @param pos	������������Ӿ���Ӿ����Ӿ伯�е�λ��
	 * @return		����ansno
	 * */
	int oneResolution(int pos);

	/************
	 * �ж�����ԭ�Ӿ��Ƿ������һ����
	 * @param a1	ԭ�Ӿ�1
	 * @param a2	ԭ�Ӿ�2
	 * */
	bool canMerge(const Atomic_clause &a1, const Atomic_clause &a2);

	/************
	 * �������Ӿ�����û���һ
	 * @param father	�����û���һ�ĵ�һ���Ӿ����Ӿ伯���±�
	 * @param mother	�����û���һ�ĵڶ����Ӿ����Ӿ伯���±�
	 * @param _del		�û���һʱ�ϵ���ԭ�Ӿ��ڵڶ����Ӿ��е��±�
	 * @return		����ansno
	 * */
	int mergeClause(int father, int mother, int _del);

	/************
	 * ���ڵݹ������Ჽ��
	 * @param c	��ǰҪ������Ӿ�
	 * */
	void _outSteps(const Clause &c);

public:
	/************
	 * �ղι��죬���ڳ�ʼ�����ڱ����������ʾ��������ʾ���������Ӿ�ͽ���
	 * */
	Resolution();

	/************
	 * �����������������ʵ�ֹ���㷨
	 * @return	����ansno
	 * */
	int resolutionSearch();

	/************
	 * �Ƚ����жϺͳ�ʼ�����ڵ���_outStep�����ݹ������Ჽ��
	 * @return	�ܷ������Ჽ�裬��ansno==NO_ANS�������
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
	cout << "�����������Ӿ�('|'�����'!'��'~'�����)��������ɺ����һ������'#'��ʾ�������\n";
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
		 << "(��ֱ�Ӹ���ʾ��)\n";
	for (;;)
	{
		cin >> temp_s;
		if (temp_s == "#")
			break;
		if (!addClause(temp_s))
		{
			// cerr << "wrong clause, please input again\n";
			cerr << "�Ӿ������������������룡\n";
			// continue;
		}
	}
	cout << "������һ�������Ӿ�\n";
	cout << "Example : \n"
		 << "Kill(A,A)\n"
		 << "(��ֱ�Ӹ���ʾ��)\n";
	cin >> temp_s;
	while (!addClause(temp_s, 1))
	{
		// cerr << "wrong clause, please input again\n";
		cerr << "�Ӿ������������������룡\n";
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