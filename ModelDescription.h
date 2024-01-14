#include <stdlib.h>
#include <math.h>

#include <vector>

using namespace std;

#define URND(max) fabs(rand()%(int)(max*100.0f)/100.0f) //��������� �������������� �������� �������� � �������� ���������
#define RND(max) -max+fabs(rand()%(int)(max*2*100.0f)/100.0f) //��������� �������� �������� � ��������� �� -max �� +max

int spriteMode = 1; //����� ���������� ������� (0, 1, 2)

//��������� ��������� ������
float vXZ0 = 0.05f, vY0  = 1.0f; 

struct SPRITE
{
	GLfloat p[3]; //��������� �������
	GLfloat c[4]; //���� �������
	GLfloat s; //������ �������

	GLfloat v[3]; //������ ��������� �������
	GLfloat g; //������ ���� �������

	GLfloat t; //����� ����� �������, ������ ��� ������ 3

	void step()
	{
		if (spriteMode == 1)
		{
			//�������� ��������� ������� �������� �������� ���������
			p[0] += v[0];
			p[1] += v[1] + g;
			p[2] += v[2];
		
			v[1] *= 0.99f; //��������� ������������ ���������
	
			g *= 1.01f; //����������� ���� �������
			
			//���� ������� �������� 0 �� ��������� ������ ����� ��������� �������
			if (p[1] <= 0.0f) reset();
		}
		else if (spriteMode == 2)
		{
			p[0] += v[0];
			p[1] += v[1] + g;
			p[2] += v[2];
	
			v[1] *= 0.99f;
			g *= 1.01f;

			c[3] *= 0.9f; //����������� ������������ �������

			s *= 1.05f; //����������� ������
			
			if (p[1] <= 0.0f) reset();
		}
		else if (spriteMode == 3)
		{
			if (t > 0.1f)
			{
				if (p[1] >= 0.0f)
				{
					p[0] += v[0];				
					p[1] += v[1] + g;
					p[2] += v[2];

					if (p[1] < 0.0f) //���� ������� ����� ���� ���������� �������� �� �����������
					{
						v[0] = RND(0.1f);
						v[2] = RND(0.1f);
					}
				}
				else
				{
					p[0] += v[0];
					p[2] += v[2];

					c[3] *= 0.97f;

					s *= 1.03f;
				}

				t *= 0.93f;
			}
			else reset();
		
			v[1] *= 0.99f;
			g *= 1.01f;
		}
		else if (spriteMode == 4) //task
		{
			p[0] += v[0];
			p[1] += v[1] + g;
			p[2] += v[2];

			v[0] *= 0.99f;
			v[1] *= 0.99f;
			v[2] *= 0.99f;
			g *= 1.009f;

			c[3] *= 1.1f; //����������� ������������ �������

			s *= 1.05f; //����������� ������

			if (p[0] >= 7.0f || p[1] >= 7.0f || p[2] >= 7.0f || p[0] <= -7.0f || p[1] <= -7.0f || p[2] <= -7.0f) 
			{
				p[0] += v[0];
				p[1] += v[1] + g;
				p[2] += v[2];

				v[0] *= 0.99f;
				v[1] *= 0.99f;
				v[2] *= 0.99f;
				g *= 1.009f;

				c[3] *= 0.9f; //����������� ������������ �������

				s *= 0.95f; //����������� ������
				if (p[0] >= 10.0f || p[1] >= 10.0f || p[2] >= 10.0f || p[0] <= -10.0f || p[1] <= -10.0f || p[2] <= -10.0f) 
				{
					reset();
				}
			}
		}
	}

	void reset()
	{
		if (spriteMode == 1)
		{
			c[0] = URND(1.0f);
			c[1] = URND(1.0f);
			c[2] = 1.0f - c[0];
			c[3] = 0.5f+URND(0.5f);

			p[0] = 0.0f; p[1] = 0.0f; p[2] = 0.0f;	

			s = 0.5f+URND(0.5f);

			v[0] = RND(vXZ0); 
			v[1] = URND(vY0); 
			v[2] = RND(vXZ0);
		}
		else if (spriteMode == 2)
		{
			c[0] = 1.0f;
			c[1] = 0.4+RND(0.2f);
			c[2] = URND(0.2f);
			c[3] = 0.6f+RND(0.2f);

			p[0] = 0.0f; p[1] = 0.0f; p[2] = 0.0f;	

			s = 0.5f + URND(0.5f);

			v[0] = RND(vXZ0); 
			v[1] = URND(vY0); 
			v[2] = RND(vXZ0);
		}
		else if (spriteMode == 3)
		{
			c[0] = URND(0.2f);
			c[1] = 0.4+RND(0.2f);
			c[2] = 1.0f;
			c[3] = 0.5f+URND(0.5f);

			p[0] = 0.0f; p[1] = 0.0f; p[2] = 0.0f;

			s = 0.5f+URND(0.5f);

			v[0] = RND(vXZ0); 
			v[1] = URND(vY0); 
			v[2] = RND(vXZ0);
		}		
		else if (spriteMode == 4) //task
		{
			c[0] = 1.0f + RND(0.1f);
			c[1] = 0.7f + RND(0.02f);
			c[2] = 0.3f + RND(0.02f);
			c[3] = 0.5f + RND(0.02f);

			p[0] = 0.0f; p[1] = 0.0f; p[2] = 0.0f;

			s = 0.1f + URND(0.1f);

			v[0] = RND(0.5f);
			v[1] = RND(1.0f);
			v[2] = RND(0.5f);
		}

		t = URND(400.0f);

		g = -0.5f;
	}

	SPRITE() //�����������
	{
		reset();
	}
};

typedef vector<SPRITE> SPRITES;

SPRITES modelSprites;

void modelInitSprites()
{
	modelSprites.clear();

	//�������������� ������� �� 100 ������
	for (int i = 0; i < 300; i++)
		modelSprites.push_back(SPRITE());
}