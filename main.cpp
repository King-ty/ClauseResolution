#include "Resolution.hpp"
using namespace std;

int main()
{
	Resolution reso;				   //定义并初始化子句归结类
	int ret = reso.resolutionSearch(); //进行归结搜索
	//如果归结得到结果，输出成功提示及归结步骤
	if (ret == 1)
	{
		cout << "归结出结论，具体归结过程如下：\n";
		reso.outSteps();
	}
	//如果归结得到悖论，输出相应提示及归结步骤
	else if (ret == -1)
	{
		cout << "归结出矛盾，具体归结过程如下：\n";
		reso.outSteps();
	}
	//如果无法归结到结果，输出提示
	else
	{
		cout << "无法归结出结论！\n";
	}

	return 0;
}