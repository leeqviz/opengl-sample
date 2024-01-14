
#define MOUSE_SENSITIVITY 0.5f; //чувствительность поворота камеры
#define WHEEL_SENSITIVITY 0.5f; //чувствительность приближения камеры

class CameraMotionHelper //класс камеры
{
private:
	bool leftButtonDown; //переменная хранит состояние левой кнопки мыши

	int currentPosition[2]; //переменные для хранения текущей и предыдущей позиции курсора (х и у)
	int lastPosition[2];

	float rotationY; //переменные для хранения углов поворота сцены вычисленные из позиций курсора
	float rotationXZ;

	float distance; //расстояние от наблюдателя до центра сцены

public: //методы для доступа к значениям переменных
	float getRotationY() { return rotationY; }
	float getRotationXZ() { return rotationXZ; }
	float getDistance() { return distance; }

public:
	void startMove(int x, int y) //нажатие левой кнопки
	{
		leftButtonDown = true;
		lastPosition[0] = x;
		lastPosition[1] = y;
		currentPosition[0] = x;
		currentPosition[1] = y;
	}

	void move(int x, int y) //движение мыши с нажатой левой кнопкой
	{
		if (leftButtonDown)
		{
			lastPosition[0] = currentPosition[0];
			lastPosition[1] = currentPosition[1];

			currentPosition[0] = x;
			currentPosition[1] = y;

			rotationY += (currentPosition[0] - lastPosition[0]) * MOUSE_SENSITIVITY;
			rotationXZ += (currentPosition[1] - lastPosition[1]) * MOUSE_SENSITIVITY;
		}
	}

	void endMove() //отжатие левой кнопки
	{
		leftButtonDown = false;
	}

	void increaseDistance() { distance += WHEEL_SENSITIVITY; } //методы для увеличения и уменьшения дистанции от наблюдателя до наблюдаемой точки методы вызываются при вращении колеса мыши
	void decreaseDistance() { distance -= WHEEL_SENSITIVITY; }

public: //callbacks

	void mouseWheelCallback(int button, int dir, int x, int y) //метод получения событий от колеса мыши используется для приближения и отдаления камеры наблюдателя
	{
		if (dir < 0) increaseDistance(); 
		else decreaseDistance();
	}

public:

	CameraMotionHelper() //конструктор
	{
		leftButtonDown = false;

		lastPosition[0] = 0; 
		lastPosition[1] = 0;
		currentPosition[0] = 0; 
		currentPosition[1] = 0;

		rotationY = 0.0f;
		rotationXZ = 0.0f;

		distance = 13.0f;
	}
};