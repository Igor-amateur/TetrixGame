// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#pragma once

#include <random> 
#include <ctime>

#include "BaseApp.h"

const int field_width = 25;
const int field_height = 26;

class TetrixGame : public BaseApp
{
	typedef BaseApp Parent;

private:
	std::mt19937 m_gen;
	int m_figure_type;
	int m_figure_new_type;
	int m_Obj1XOld;
	int m_Obj1YOld;
	int m_Obj1X;
	int m_Obj1Y;

	int m_Obj2X;
	int m_Obj2Y;
	int m_score;
	
	bool m_Direction;
	bool m_init;
	double m_rotate;
	double m_old_rotate;
	double m_time_passed;

	const int m_figures[7][4];
	const wchar_t m_string_type_1[field_width];
	const wchar_t m_string_type_2[field_width];
	const wchar_t m_string_type_3[field_width];

	std::wstring m_message;

	POINT m_new_points[4], m_old_points[4], m_previous_points[4];

private:
	int Get_type_tetramino();
	void Field_initialization();
	void Initialization_new_tetramino();
	void Horizontal_displacement_tetramino();
	void Vertical_displacement_tetramino();
	void Rotate_tetramino();
	void Draw_tetramino();
	void Remove_tetramino();

	bool Check_transformation();
	void Decline_transformation();
	void Done_transformation();
	void Init_previous_tetramino();
	void Show_previous_tetramino();
	void Remove_previous_tetramino();

	void Drow_message(int x, int y, std::wstring mes);

	bool Char_exist(int y, wchar_t ch);
	int Score_count();
	void Redraw_field(int y);
	void Check_skore_field_redraw();
public:
	TetrixGame();
	virtual void KeyPressed(int btnCode);
	virtual void UpdateF(float deltaTime);
};
