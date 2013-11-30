#pragma once

struct Chara{

	virtual bool update() = 0;
	virtual void draw( int, int ) = 0;

	virtual ~Chara(){}

};