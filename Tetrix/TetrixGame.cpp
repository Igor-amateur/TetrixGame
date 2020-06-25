// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#include "TetrixGame.h"


constexpr bool GOOD_TRANSFORMATION(true);
constexpr int START_X (7);
constexpr int START_Y(1);
constexpr wchar_t QUARTER{ wchar_t(254) };
constexpr wchar_t POINT_{'.'};

TetrixGame::TetrixGame() : Parent(field_width - 1, field_height - 1), 
m_gen(static_cast<unsigned int>(time(0))),
m_figures{ 1,3,5,7, // I
		2,4,5,7, // S
		3,5,4,6, // Z
		3,5,4,7, // T
		2,3,5,7, // L
		3,5,7,6, // J
		2,3,4,5, // O
},
m_string_type_1{ L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#',
L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#', L'#',L'#',L'#' },
m_string_type_2{ L'#', L'.', L'.', L'.', L'.', L'.', L'.', L'.', L'.',
L'.', L'.', L'.', L'.', L'.', L'.', L'.', L'#', L' ', L' ', L' ', L' ', L' ', L' ',L' ',L'#' },
m_string_type_3{ L'#', L' ', L' ', L' ', L' ', L' ', L' ', L' ', L' ',
L' ', L' ', L' ', L' ', L' ', L' ', L' ', L' ', L' ', L' ', L' ', L' ', L' ', L' ',L' ',L'#' }
{
	m_figure_type = Get_type_tetramino();
	m_figure_new_type = Get_type_tetramino();
	m_init = false;
	m_time_passed = 0.0;
	m_rotate = 0.0;
	m_old_rotate = 0.0;
	m_Direction = true;
	m_Obj1XOld = m_Obj1X = START_X;
	m_Obj1YOld = m_Obj1Y = START_Y;
	m_Obj2X = 20;
	m_Obj2Y = 1;
	m_score = 0;
	m_message = L"> Score: ";
	Field_initialization();
}

double get_PI()
{
	double pi;
	__asm
	{
		fldpi
		fstp pi
	}
	return pi;
}

int TetrixGame::Get_type_tetramino()
{
	std::uniform_int_distribution<int> tt_id(0, 6);
	return tt_id(m_gen);
}

void TetrixGame::KeyPressed(int btnCode)
{
	if (btnCode == 32) //w
	{
		m_rotate = m_rotate + 90.0 * get_PI() / 180.0;
		if (m_rotate > 4.7124)
			m_rotate = 0.0;
	}
	else if (btnCode == 80) //s
	{
		m_Obj1Y++;
	}
	else if (btnCode == 75) //a
	{
		m_Obj1X--;
	}
	else if (btnCode == 77) //d
	{
		m_Obj1X++;
	}
}


bool TetrixGame::Check_transformation()
{
	int result_true(0);
	int result_false(0);
	for (int i = 0; i < 4; i++)
	{
		int x = m_new_points[i].x;
		int y = m_new_points[i].y;
		if (L'.' != GetChar(x, y))
		{
			++result_false;
			for (int k = 0; k < 4; ++k)
			{
				if(m_old_points[k].x == x && m_old_points[k].y == y)
					++result_true;
			}
		}
	}
	return result_true >= result_false;
}

void TetrixGame::Decline_transformation()
{
	for (int i = 0; i < 4; i++)
	{
		// Отмена перемещения
		m_new_points[i].x = m_old_points[i].x;
		m_new_points[i].y = m_old_points[i].y;
	}
}

void TetrixGame::Done_transformation()
{
	for (int i = 0; i < 4; i++)
	{
		// Сохранение перемещения
		m_old_points[i].x = m_new_points[i].x;
		m_old_points[i].y = m_new_points[i].y;
	}

}

void TetrixGame::Initialization_new_tetramino()
{
	for (int i = 0; i < 4; i++)
	{
		m_new_points[i].x = m_figures[m_figure_type][i] % 2;
		m_new_points[i].y = m_figures[m_figure_type][i] / 2;
	}
}

void TetrixGame::Horizontal_displacement_tetramino()
{
	for (int i = 0; i < 4; i++)
	{
		// Трансформации перемещения
		m_new_points[i].x = m_new_points[i].x + m_Obj1X;
	}
	m_Obj1X = 0;
}

void TetrixGame::Vertical_displacement_tetramino()
{
	for (int i = 0; i < 4; i++)
	{
		// Трансформации перемещения
		m_new_points[i].y = m_new_points[i].y + m_Obj1Y;
	}
	m_Obj1Y = 0;
}

void TetrixGame::Rotate_tetramino()
{
	//Определяем центр вращения
	POINT pNew = m_new_points[1];
	for (int i = 0; i < 4; i++)
	{
		// Трансформации вращения
//		m_new_points[i].x = pNew.x + (m_old_points[i].x - pNew.x) * cos(rotate) - (m_old_points[i].y - pNew.y) * sin(rotate); //
		m_new_points[i].x = pNew.x - (m_old_points[i].y - pNew.y); // Каждый поворот 90 градусов
//		m_new_points[i].y = pNew.y + (m_old_points[i].y - pNew.y) * cos(rotate) + (m_old_points[i].x - pNew.x) * sin(rotate); //
		m_new_points[i].y = pNew.y + (m_old_points[i].x - pNew.x); //
	}
	m_rotate = 0.0;
}

void TetrixGame::Draw_tetramino()
{
	for (int i = 0; i < 4; i++)
	{
		// 
		int x = m_new_points[i].x; //
		int y = m_new_points[i].y; //
		// Чистим поле с предыдущей позицией фигуры
		SetChar(x, y, QUARTER);
	}
}

void TetrixGame::Remove_tetramino()
{
	for (int i = 0; i < 4; i++)
	{
		// 
		int x = m_old_points[i].x; //
		int y = m_old_points[i].y; //
		// Чистим поле с предыдущей позицией фигуры
		SetChar(x, y, POINT_);
	}
}

void TetrixGame::UpdateF(float deltaTime)
{

	m_time_passed = m_time_passed + deltaTime;

	if (m_time_passed > 0.4)
	{
		m_time_passed = 0.0;
		++m_Obj1Y;
	}
	//Если первый запуск 
	if (false == m_init)
	{
		// Перемещаем на исходную позицию
		Horizontal_displacement_tetramino();
		Vertical_displacement_tetramino();
		// // Если трансформация корректна
		if (Check_transformation() == GOOD_TRANSFORMATION)
		{
			// инициализируем копию тетрамино
			Done_transformation();
			// Маркируеми первый запуск
			m_init = true;
			// Отрисовываем тетрамино
			Draw_tetramino();

			//=====================================

			// Подготавливаем рандомно фигуру тетрамино
			m_figure_new_type = Get_type_tetramino();
			// Создаем привьюшку следующей фигуры
			Init_previous_tetramino();
			Show_previous_tetramino();

			//================================================
//
			std::wstring message = m_message + std::to_wstring(m_score);

			Drow_message(3, 23, message);

			return;
		}
		else
		{
			exit(0);
		}
	}
	else
	{
		if (m_Obj1X != 0)
		{
			// Применяем горизонтальное перемещение
			Horizontal_displacement_tetramino();
			// // Если трансформация корректна
			if (Check_transformation() == GOOD_TRANSFORMATION)
			{
				//Удаляем старое изображение тетрамино
				Remove_tetramino();
				//Сохраняем полученое тетрамино
				Done_transformation();
			}
			else
			{
				// Иначе отменяем трансформацию
				Decline_transformation();
			}
		}
	}

	if (m_Obj1Y != 0)
	{
		// Применяем вертикальное перемещение
		Vertical_displacement_tetramino();

		// Если трансформация корректна
		if (Check_transformation() == GOOD_TRANSFORMATION)
		{
			//Удаляем старое изображение тетрамино
			Remove_tetramino();
			//Сохраняем полученое тетрамино
			Done_transformation();
		}
		else
		{
			Remove_previous_tetramino();

			Check_skore_field_redraw();

			m_Obj1XOld = m_Obj1X = START_X;
			m_Obj1YOld = m_Obj1Y = START_Y;
			m_figure_type = m_figure_new_type;
			// создаем новый тетрамино
			Initialization_new_tetramino();
			//Сохраняем его копию
			Done_transformation();
			m_init = false;
			return;

			// Перемещаем на исходную позицию
			Horizontal_displacement_tetramino();
			Vertical_displacement_tetramino();
			//Сохраняем полученое тетрамино
			Done_transformation();
			// Отрисовываем тетрамино
			Draw_tetramino();
			//=====================================
			// Подготавливаем рандомно фигуру тетрамино
			m_figure_new_type = Get_type_tetramino();
			// Создаем привьюшку следующей фигуры
			Init_previous_tetramino();
			Show_previous_tetramino();
			return;
		}
	}

	if (m_rotate > 0.0)
	{
		// Пробуем выполнить поворот тетрамино
		Rotate_tetramino();
		// // Если трансформация корректна
		if (Check_transformation() == GOOD_TRANSFORMATION)
		{
			//Удаляем старое изображение тетрамино
			Remove_tetramino();
			//Сохраняем полученое тетрамино
			Done_transformation();
		}
		else
		{
			// Иначе отменяем трансформацию
			Decline_transformation();
		}
		// Отрисовываем тетрамино
	}
	Draw_tetramino();
}

void TetrixGame::Init_previous_tetramino()
{
	for (int i = 0; i < 4; i++)
	{
		m_previous_points[i].x = m_figures[m_figure_new_type][i] % 2;
		m_previous_points[i].y = m_figures[m_figure_new_type][i] / 2;
	}

	for (int i = 0; i < 4; i++)
	{
		// Трансформации перемещения
		m_previous_points[i].x = m_previous_points[i].x + 20;
		m_previous_points[i].y = m_previous_points[i].y + 7;
	}
}

void TetrixGame::Remove_previous_tetramino()
{
	for (int i = 0; i < 4; i++)
	{
		// 
		int x = m_previous_points[i].x; //
		int y = m_previous_points[i].y; //
		// Чистим поле с предыдущей позицией фигуры
		SetChar(x, y, wchar_t(' '));
	}

}

void TetrixGame::Show_previous_tetramino()
{
	for (int i = 0; i < 4; i++)
	{
		// 
		int x = m_previous_points[i].x; //
		int y = m_previous_points[i].y; //
		// Чистим поле с предыдущей позицией фигуры
		SetChar(x, y, QUARTER);
	}

}

void TetrixGame::Field_initialization()
{
	POINT p;
	p.x = 0;
	for (int x_pos = 0; x_pos < field_width; ++x_pos)
	{
		/*0*/SetChar(x_pos, /* y_pos =*/0/**/, m_string_type_1[x_pos]);
		/*1*/SetChar(x_pos, /* y_pos =*/1/**/, m_string_type_2[x_pos]);
		/*2*/SetChar(x_pos, /* y_pos =*/2/**/, m_string_type_2[x_pos]);
		/*3*/SetChar(x_pos, /* y_pos =*/3/**/, m_string_type_2[x_pos]);
		/*4*/SetChar(x_pos, /* y_pos =*/4/**/, m_string_type_2[x_pos]);
		/*5*/SetChar(x_pos, /* y_pos =*/5/**/, m_string_type_2[x_pos]);
		/*6*/SetChar(x_pos, /* y_pos =*/6/**/, m_string_type_2[x_pos]);
		/*7*/SetChar(x_pos, /* y_pos =*/7/**/, m_string_type_2[x_pos]);
		/*8*/SetChar(x_pos, /* y_pos =*/8/**/, m_string_type_2[x_pos]);
		/*9*/SetChar(x_pos, /* y_pos =*/9/**/, m_string_type_2[x_pos]);
		/*10*/SetChar(x_pos, /* y_pos =*/10/**/, m_string_type_2[x_pos]);
		/*11*/SetChar(x_pos, /* y_pos =*/11/**/, m_string_type_2[x_pos]);
		/*12*/SetChar(x_pos, /* y_pos =*/12/**/, m_string_type_2[x_pos]);
		/*13*/SetChar(x_pos, /* y_pos =*/13/**/, m_string_type_2[x_pos]);
		/*14*/SetChar(x_pos, /* y_pos =*/14/**/, m_string_type_2[x_pos]);
		/*15*/SetChar(x_pos, /* y_pos =*/15/**/, m_string_type_2[x_pos]);
		/*16*/SetChar(x_pos, /* y_pos =*/16/**/, m_string_type_2[x_pos]);
		/*17*/SetChar(x_pos, /* y_pos =*/17/**/, m_string_type_2[x_pos]);
		/*18*/SetChar(x_pos, /* y_pos =*/18/**/, m_string_type_2[x_pos]);
		/*19*/SetChar(x_pos, /* y_pos =*/19/**/, m_string_type_2[x_pos]);
		/*20*/SetChar(x_pos, /* y_pos =*/20/**/, m_string_type_2[x_pos]);
		/*21*/SetChar(x_pos, /* y_pos =*/21/**/, m_string_type_1[x_pos]);
		/*22*/SetChar(x_pos, /* y_pos =*/22/**/, m_string_type_3[x_pos]);
		/*23*/SetChar(x_pos, /* y_pos =*/23/**/, m_string_type_3[x_pos]);
		/*24*/SetChar(x_pos, /* y_pos =*/24/**/, m_string_type_3[x_pos]);
		/*25*/SetChar(x_pos, /* y_pos =*/25/**/, m_string_type_1[x_pos]);
	}
	
	Initialization_new_tetramino();
}

void TetrixGame::Drow_message(int x, int y, std::wstring mes)
{
	for(int i = 0; i < int(mes.size()); ++i)
	SetChar(x + i, y, mes[i]);
}

bool TetrixGame::Char_exist(int y, wchar_t ch)
{
	int start_x(1), end_x(16);
	for(int x = start_x; x < end_x; ++x)
	{
		if (GetChar(x, y) == ch)
			return true;
	}
	return false;
}

int TetrixGame::Score_count()
{
	int start_y(20), end_y(1);
	int result(0);
	for (int y = start_y; y > end_y; --y)
	{
		if (false == Char_exist(y, POINT_))
		{
			++result;
		}
	}
	return result;
}

void TetrixGame::Check_skore_field_redraw()
{
	int start_y(20), end_y(1);
	int result(0);
	for (int y = start_y; y > end_y; --y)
	{
		if (false == Char_exist(y, POINT_))
		{
			Redraw_field(y);
			result += 100;
			++y;
		}
	}
	if(result > 0)
	m_score += result;
}

void TetrixGame::Redraw_field(int s_y)
{
	int start_y(s_y),end_y(1);
	int start_x(1), end_x(16);
	for (int y = start_y; y > end_y; --y)
	{
		for (int x = start_x; x < end_x; ++x)
		{
			SetChar(x, y, GetChar(x, y - 1));
		}

	}
}