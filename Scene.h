#pragma once

namespace Scene{
enum E{
	TITLE,
	ACTION,
	SCENE_NUM
};
}

typedef Scene::E SceneE;


extern void setScene( SceneE );
extern void process();
