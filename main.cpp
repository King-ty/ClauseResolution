#include "Resolution.hpp"
using namespace std;

int main()
{
	Resolution reso;				   //���岢��ʼ���Ӿ�����
	int ret = reso.resolutionSearch(); //���й������
	//������õ����������ɹ���ʾ����Ჽ��
	if (ret == 1)
	{
		cout << "�������ۣ�������������£�\n";
		reso.outSteps();
	}
	//������õ���ۣ������Ӧ��ʾ����Ჽ��
	else if (ret == -1)
	{
		cout << "����ì�ܣ�������������£�\n";
		reso.outSteps();
	}
	//����޷���ᵽ����������ʾ
	else
	{
		cout << "�޷��������ۣ�\n";
	}

	return 0;
}