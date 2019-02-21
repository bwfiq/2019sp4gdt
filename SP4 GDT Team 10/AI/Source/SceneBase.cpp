#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include <sstream>
#include "SceneData.h"

static const int fontWidth[] = { 0,26,26,26,26,26,26,26,26,26,26,26,26,0,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,12,17,21,26,26,37,35,11,16,16,26,26,13,16,13,20,26,26,26,26,26,26,26,26,26,26,14,14,26,26,26,24,46,30,28,28,32,25,24,33,32,13,17,27,22,44,34,34,27,35,28,24,25,33,30,46,27,25,24,16,20,16,26,26,15,25,27,22,27,26,16,24,27,12,12,24,12,42,27,27,27,27,18,20,17,27,23,37,23,24,21,16,24,16,26,26,26,26,13,16,22,36,26,26,21,54,24,18,45,26,24,26,26,13,13,22,22,26,26,47,23,37,20,18,44,26,21,25,12,17,26,26,26,26,26,26,20,43,21,27,26,16,26,20,18,26,17,17,15,29,30,13,16,13,22,27,33,35,35,24,30,30,30,30,30,30,40,28,25,25,25,25,13,13,13,13,32,34,34,34,34,34,34,26,35,33,33,33,33,25,27,27,25,25,25,25,25,25,40,22,26,26,26,26,12,12,12,12,27,27,27,27,27,27,27,26,28,27,27,27,27,24,27,24 };

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	// Black background
	glClearColor(0.13333f, 0.17254f, 0.21568f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//comg.vertexshader", "Shader//comg.fragmentshader" );
	m_gPassShaderID = LoadShaders("Shader//GPass.vertexshader", "Shader//GPass.fragmentshader");
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");
	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		std::string texten = "colorTextureEnabled[" + std::to_string(i) + "]";
		std::string text = "colorTexture[" + std::to_string(i) + "]";
		m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED + i] =
			glGetUniformLocation(m_gPassShaderID, texten.c_str());
		m_parameters[U_SHADOW_COLOR_TEXTURE + i] =
			glGetUniformLocation(m_gPassShaderID, text.c_str());
	}
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE1_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	m_parameters[U_COLOR_TEXTURE2_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[2]");
	m_parameters[U_COLOR_TEXTURE2] = glGetUniformLocation(m_programID, "colorTexture[2]");
	m_parameters[U_COLOR_TEXTURE3_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[3]");
	m_parameters[U_COLOR_TEXTURE3] = glGetUniformLocation(m_programID, "colorTexture[3]");
	m_parameters[U_COLOR_TEXTURE4_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[4]");
	m_parameters[U_COLOR_TEXTURE4] = glGetUniformLocation(m_programID, "colorTexture[4]");
	m_parameters[U_COLOR_TEXTURE5_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[5]");
	m_parameters[U_COLOR_TEXTURE5] = glGetUniformLocation(m_programID, "colorTexture[5]");
	m_parameters[U_COLOR_TEXTURE6_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[6]");
	m_parameters[U_COLOR_TEXTURE6] = glGetUniformLocation(m_programID, "colorTexture[6]");
	m_parameters[U_COLOR_TEXTURE7_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[7]");
	m_parameters[U_COLOR_TEXTURE7] = glGetUniformLocation(m_programID, "colorTexture[7]");
	m_parameters[U_UV_OFFSET] = glGetUniformLocation(m_programID, "uvOffset");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	m_parameters[U_ALPHA] = glGetUniformLocation(m_programID, "alpha");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 5.f);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1.f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform2f(m_parameters[U_UV_OFFSET], 0, 0);

	//camera.Init(Vector3(-3, 3, 3), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(Vector3(0, 2, 2), Vector3(0, 0, 0), Vector3(0, 1, 0));
	Vector3 dir = camera.target - camera.position;
	dir.Normalize();
	Vector3 right(1, 0, 0);
	camera.up = right.Cross(dir);
	camera.fDistance = (camera.target - camera.position).Length();

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	m_lightDepthFBO.Init(2048*6, 2048*6);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f);
	meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();
	meshList[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(1, 0, 0), 10, 10, 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 1.f);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureArray[0] = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT_GUNGSUH] = MeshBuilder::GenerateText("gungsuh", 16, 16);
	meshList[GEO_TEXT_GUNGSUH]->textureArray[0] = LoadTGA("Image//gungsuh.tga");
	meshList[GEO_BG] = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1));
	meshList[GEO_BG]->textureArray[0] = LoadTGA("Image//Village.tga");
	meshList[GEO_CROSS] = MeshBuilder::GenerateQuad("cross", Color(1, 1, 1));
	meshList[GEO_CROSS]->textureArray[0] = LoadTGA("Image//cross.tga");
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateQuad("circle", Color(1, 1, 1));
	meshList[GEO_CIRCLE]->textureArray[0] = LoadTGA("Image//circle.tga");
	meshList[GEO_TOOFULL] = MeshBuilder::GenerateQuad("toofull", Color(1, 1, 1));
	meshList[GEO_TOOFULL]->textureArray[0] = LoadTGA("Image//toofull.tga");
	meshList[GEO_FULL] = MeshBuilder::GenerateQuad("full", Color(1, 1, 1));
	meshList[GEO_FULL]->textureArray[0] = LoadTGA("Image//full.tga");
	meshList[GEO_HUNGRY] = MeshBuilder::GenerateQuad("hungry", Color(1, 1, 1));
	meshList[GEO_HUNGRY]->textureArray[0] = LoadTGA("Image//hungry.tga");
	meshList[GEO_DEAD] = MeshBuilder::GenerateQuad("dead", Color(1, 1, 1));
	meshList[GEO_DEAD]->textureArray[0] = LoadTGA("Image//dead.tga");
	meshList[GEO_SHARK] = MeshBuilder::GenerateQuad("shark", Color(1, 1, 1));
	meshList[GEO_SHARK]->textureArray[0] = LoadTGA("Image//shark.tga");
	meshList[GEO_FISHFOOD] = MeshBuilder::GenerateQuad("fishfood", Color(1, 1, 1));
	meshList[GEO_FISHFOOD]->textureArray[0] = LoadTGA("Image//fishfood.tga");
	meshList[GEO_CRAZY] = MeshBuilder::GenerateQuad("crazy", Color(1, 1, 1));
	meshList[GEO_CRAZY]->textureArray[0] = LoadTGA("Image//crazy.tga");
	meshList[GEO_HAPPY] = MeshBuilder::GenerateQuad("happy", Color(1, 1, 1));
	meshList[GEO_HAPPY]->textureArray[0] = LoadTGA("Image//happy.tga");

	meshList[GEO_QUEEN] = MeshBuilder::GenerateQuad("queen", Color(1, 1, 1));
	meshList[GEO_QUEEN]->textureArray[0] = LoadTGA("Image//queen.tga");
	meshList[GEO_CHESSBOARD] = MeshBuilder::GenerateQuad("chessboard", Color(1, 1, 1));
	meshList[GEO_CHESSBOARD]->textureArray[0] = LoadTGA("Image//chessboard.tga");

	//Assignment Images
	meshList[GEO_SOLDIER] = MeshBuilder::GenerateQuad("soldier", Color(1, 1, 1));
	meshList[GEO_SOLDIER]->textureArray[0] = LoadTGA("Image//FootSoldier.tga");
	meshList[GEO_COMMANDER] = MeshBuilder::GenerateQuad("commander", Color(1, 1, 1));
	meshList[GEO_COMMANDER]->textureArray[0] = LoadTGA("Image//Commander.tga");
	meshList[GEO_CAVALRY] = MeshBuilder::GenerateQuad("cavalry", Color(1, 1, 1));
	meshList[GEO_CAVALRY]->textureArray[0] = LoadTGA("Image//Cavalry.tga");
	meshList[GEO_ANIMAL] = MeshBuilder::GenerateQuad("animal", Color(1, 1, 1));
	meshList[GEO_ANIMAL]->textureArray[0] = LoadTGA("Image//Animal.tga");
	meshList[GEO_KING] = MeshBuilder::GenerateQuad("king", Color(1, 1, 1));
	meshList[GEO_KING]->textureArray[0] = LoadTGA("Image//King.tga");

	meshList[GEO_WHITEQUAD] = MeshBuilder::GenerateQuad("whitequad", Color(1, 1, 1));
	meshList[GEO_GREYQUAD] = MeshBuilder::GenerateQuad("greyquad", Color(0.5, 0.5, 0.5));
	meshList[GEO_BLACKQUAD] = MeshBuilder::GenerateQuad("blackquad", Color(0, 0, 0));
	meshList[GEO_REDQUAD] = MeshBuilder::GenerateQuad("redquad", Color(1, 0, 0));
	meshList[GEO_GREENQUAD] = MeshBuilder::GenerateQuad("greenquad", Color(0, 1, 0));
	meshList[GEO_YELLOWQUAD] = MeshBuilder::GenerateQuad("yellowquad", Color(1, 1, 0));

	meshList[GEO_WHITEHEX] = MeshBuilder::GenerateRing("whitequad", Color(1, 1, 1), 6, 0.5f);
	meshList[GEO_GREYHEX] = MeshBuilder::GenerateRing("whitequad", Color(0.5, 0.5, 0.5), 6, 0.5f);
	meshList[GEO_BLACKHEX] = MeshBuilder::GenerateRing("whitequad", Color(0, 0, 0), 6, 0.5f);

	meshList[GEO_RED_TRIANGLE] = MeshBuilder::GenerateTriangle("redTriangle", Color(1, 0, 0), 1.f);
	meshList[GEO_BLUE_TRIANGLE] = MeshBuilder::GenerateTriangle("blueTriangle", Color(0, 0, 1), 1.f);

	meshList[GEO_RED_MELEE] = MeshBuilder::GenerateQuad("redmelee", Color(1, 1, 1));
	meshList[GEO_RED_MELEE]->textureArray[0] = LoadTGA("Image//RedMelee.tga");
	meshList[GEO_RED_CASTER] = MeshBuilder::GenerateQuad("redcaster", Color(1, 1, 1));
	meshList[GEO_RED_CASTER]->textureArray[0] = LoadTGA("Image//RedCaster.tga");
	meshList[GEO_RED_TOWER] = MeshBuilder::GenerateQuad("redtower", Color(1, 1, 1));
	meshList[GEO_RED_TOWER]->textureArray[0] = LoadTGA("Image//RedTower.tga");
	meshList[GEO_BLUE_MELEE] = MeshBuilder::GenerateQuad("bluemelee", Color(1, 1, 1));
	meshList[GEO_BLUE_MELEE]->textureArray[0] = LoadTGA("Image//BlueMelee.tga");
	meshList[GEO_BLUE_CASTER] = MeshBuilder::GenerateQuad("bluecaster", Color(1, 1, 1));
	meshList[GEO_BLUE_CASTER]->textureArray[0] = LoadTGA("Image//BlueCaster.tga");
	meshList[GEO_BLUE_TOWER] = MeshBuilder::GenerateQuad("bluetower", Color(1, 1, 1));
	meshList[GEO_BLUE_TOWER]->textureArray[0] = LoadTGA("Image//BlueTower.tga");

	meshList[GEO_BACKGROUND] = MeshBuilder::GenerateQuad("background", Color(1, 1, 1));
	meshList[GEO_BACKGROUND]->textureArray[0] = LoadTGA("Image//background.tga");
	meshList[GEO_SPLASHSCREEN] = MeshBuilder::GenerateQuad("splashscreen", Color(1, 1, 1));
	meshList[GEO_SPLASHSCREEN]->textureArray[0] = LoadTGA("Image//splashscreen.tga");
	meshList[GEO_LOGO] = MeshBuilder::GenerateQuad("logo", Color(1, 1, 1));
	meshList[GEO_LOGO]->textureArray[0] = LoadTGA("Image//logo1.tga");

	meshList[GEO_BACKBUTTON] = MeshBuilder::GenerateQuad("backbutton", Color(1, 1, 1));
	meshList[GEO_BACKBUTTON]->textureArray[0] = LoadTGA("Image//backbutton.tga");
	meshList[GEO_STARTBUTTON] = MeshBuilder::GenerateQuad("start", Color(1, 1, 1));
	meshList[GEO_STARTBUTTON]->textureArray[0] = LoadTGA("Image//start.tga");
	meshList[GEO_OPTIONSBUTTON] = MeshBuilder::GenerateQuad("optionsbutton", Color(1, 1, 1));
	meshList[GEO_OPTIONSBUTTON]->textureArray[0] = LoadTGA("Image//optionsbutton.tga");
	meshList[GEO_QUITBUTTON] = MeshBuilder::GenerateQuad("quitbutton", Color(1, 1, 1));
	meshList[GEO_QUITBUTTON]->textureArray[0] = LoadTGA("Image//quitbutton.tga");

	meshList[GEO_SEA] = MeshBuilder::GenerateQuad("sea", Color(1, 1, 1));
	meshList[GEO_SEA]->textureArray[0] = LoadTGA("Image//sea.tga");
	//meshList[GEO_GRASS] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 1.f);
	meshList[GEO_ISLAND] = MeshBuilder::GenerateOBJ("island", "Obj//island.obj");
	meshList[GEO_ISLAND]->textureArray[0] = LoadTGA("Image//island.tga");
	meshList[GEO_GRASS] = MeshBuilder::GenerateOBJ("grass", "Obj//Ground.obj");
	meshList[GEO_GRASS]->textureArray[0] = LoadTGA("Image//grass.tga");
	meshList[GEO_TREE] = MeshBuilder::GenerateOBJ("tree", "Obj//Tree.obj");
	meshList[GEO_TREE]->textureArray[0] = LoadTGA("Image//tree.tga");
	meshList[GEO_HALFTREE] = MeshBuilder::GenerateOBJ("halftree", "Obj//HalfTree.obj");
	meshList[GEO_HALFTREE]->textureArray[0] = LoadTGA("Image//halftree.tga");
	meshList[GEO_BUSH] = MeshBuilder::GenerateOBJ("bush", "Obj//bushes.obj");
	meshList[GEO_BUSH]->textureArray[0] = LoadTGA("Image//bushes_and_berries_texture.tga");
	meshList[GEO_BERRIES] = MeshBuilder::GenerateOBJ("berry", "Obj//berries.obj");
	meshList[GEO_BERRIES]->textureArray[0] = LoadTGA("Image//bushes_and_berries_texture.tga");
	meshList[GEO_MOUNTAIN] = MeshBuilder::GenerateOBJ("mountain", "Obj//mountain.obj");
	meshList[GEO_MOUNTAIN]->textureArray[0] = LoadTGA("Image//mountain.tga");

	meshList[GEO_VILLAGER] = MeshBuilder::GenerateOBJ("villager", "Obj//villager.obj");
	meshList[GEO_VILLAGER]->textureArray[0] = LoadTGA("Image//villager.tga");
	meshList[GEO_HATCHET] = MeshBuilder::GenerateOBJ("hatchet", "Obj//hatchet.obj");
	meshList[GEO_HATCHET]->textureArray[0] = LoadTGA("Image//hatchet.tga");
	meshList[GEO_BASKET] = MeshBuilder::GenerateOBJ("basket", "Obj//basket.obj");
	meshList[GEO_BASKET]->textureArray[0] = LoadTGA("Image//basket.tga");
	meshList[GEO_TSUNAMI] = MeshBuilder::GenerateOBJ("tsunami", "Obj//tsunami.obj");
	meshList[GEO_TSUNAMI]->textureArray[0] = LoadTGA("Image//tsunami.tga");
	meshList[GEO_TSUNAMI]->material.kAmbient.Set(0, 0, 0.25f);

	meshList[GEO_BUILDING] = MeshBuilder::GenerateOBJ("building", "Obj//house.obj");
	meshList[GEO_BUILDING]->textureArray[0] = LoadTGA("Image//strawhouse.tga");
	meshList[GEO_GRANARY] = MeshBuilder::GenerateOBJ("granary", "Obj//granary.obj");
	meshList[GEO_GRANARY]->textureArray[0] = LoadTGA("Image//granary.tga");
	meshList[GEO_WOODSHED] = MeshBuilder::GenerateOBJ("woodshed", "Obj//woodshed.obj");
	meshList[GEO_WOODSHED]->textureArray[0] = LoadTGA("Image//woodshed.tga");
	meshList[GEO_BROKEN_BUILDING] = MeshBuilder::GenerateOBJ("brokenbuilding", "Obj//brokenhouse.obj");
	meshList[GEO_BROKEN_BUILDING]->textureArray[0] = LoadTGA("Image//brokenhouse.tga");
	meshList[GEO_LOGS] = MeshBuilder::GenerateOBJ("logs", "Obj//logs.obj");
	meshList[GEO_LOGS]->textureArray[0] = LoadTGA("Image//logs.tga");

	meshList[GEO_ALTAR] = MeshBuilder::GenerateOBJ("altar", "Obj//altar.obj");
	meshList[GEO_ALTAR]->textureArray[0] = LoadTGA("Image//Altar.tga");

	meshList[GEO_GRID] = MeshBuilder::GenerateGrid("grid", Color(1, 0, 0), SceneData::GetInstance()->GetNoGrid(), SceneData::GetInstance()->GetNoGrid(), SceneData::GetInstance()->GetGridSize());

	meshList[GEO_OFFERING] = MeshBuilder::GenerateQuad("offering", Color(0.5, 0.5, 0.5));
	meshList[GEO_OFFERING]->textureArray[0] = LoadTGA("Image//offeringIcon.tga");

	meshList[GEO_LINE] = MeshBuilder::GenerateLine("line", Color(1, 0, 0));
	meshList[GEO_RAY_WHITE] = MeshBuilder::GenerateRay("whiteray", Color(1, 1, 1));

	meshList[GEO_HAND_DEFAULT] = MeshBuilder::GenerateOBJ("hand_default", "Obj//hand_default.obj");
	meshList[GEO_HAND_GRAB] = MeshBuilder::GenerateOBJ("hand_grab", "Obj//hand_grab.obj");
	meshList[GEO_HAND_POINT] = MeshBuilder::GenerateOBJ("hand_point", "Obj//hand_point.obj");

	meshList[GEO_RETICLE] = MeshBuilder::GenerateReticle("reticle", Color(1, 1, 1));
	meshList[GEO_RETICLE_CROSS] = MeshBuilder::GenerateCross("reticle_cross", Color(1, 1, 1));

	meshList[GEO_UI_BOARD_BLANK] = MeshBuilder::GenerateQuad("ui_board_blank", Color());
	meshList[GEO_UI_BOARD_FOOD] = MeshBuilder::GenerateQuad("ui_board_food", Color());
	meshList[GEO_UI_BOARD_DAY] = MeshBuilder::GenerateQuad("ui_board_day", Color());
	meshList[GEO_UI_BOARD_POPULATION] = MeshBuilder::GenerateQuad("ui_board_population", Color());
	meshList[GEO_UI_BOARD_STONE] = MeshBuilder::GenerateQuad("ui_board_stone", Color());
	meshList[GEO_UI_BOARD_TIME] = MeshBuilder::GenerateQuad("ui_board_time", Color());
	meshList[GEO_UI_BOARD_WOOD] = MeshBuilder::GenerateQuad("ui_board_wood", Color());

	meshList[GEO_UI_BOARD_BLANK]->textureArray[0] = LoadTGA("Image//ui_board_blank.tga");
	meshList[GEO_UI_BOARD_FOOD]->textureArray[0] = LoadTGA("Image//ui_board_food.tga");
	meshList[GEO_UI_BOARD_DAY]->textureArray[0] = LoadTGA("Image//ui_board_day.tga");
	meshList[GEO_UI_BOARD_POPULATION]->textureArray[0] = LoadTGA("Image//ui_board_population.tga");
	meshList[GEO_UI_BOARD_STONE]->textureArray[0] = LoadTGA("Image//ui_board_stone.tga");
	meshList[GEO_UI_BOARD_TIME]->textureArray[0] = LoadTGA("Image//ui_board_time.tga");
	meshList[GEO_UI_BOARD_WOOD]->textureArray[0] = LoadTGA("Image//ui_board_wood.tga");

	meshList[GEO_WOODRESEARCH] = MeshBuilder::GenerateQuad("woodResearch", Color(1, 1, 1));
	meshList[GEO_WOODRESEARCH]->textureArray[0] = LoadTGA("Image//woodResearch.tga");
	meshList[GEO_STONERESEARCH] = MeshBuilder::GenerateQuad("stoneResearch", Color(1, 1, 1));
	meshList[GEO_STONERESEARCH]->textureArray[0] = LoadTGA("Image//stoneResearch.tga");
	meshList[GEO_FULLSTONERESEARCH] = MeshBuilder::GenerateQuad("fullStoneResearch", Color(1, 1, 1));
	meshList[GEO_FULLSTONERESEARCH]->textureArray[0] = LoadTGA("Image//fullStoneResearch.tga");
	meshList[GEO_RESEARCHTICK] = MeshBuilder::GenerateQuad("researchTick", Color(1, 1, 1));
	meshList[GEO_RESEARCHTICK]->textureArray[0] = LoadTGA("Image//researchTick.tga");

	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_TEXT_GUNGSUH]->material.kAmbient.Set(1, 0, 0);

	bLightEnabled = true;
}

void SceneBase::Update(double dt)
{
	//Keyboard Section
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (Application::IsKeyPressed('5'))
		bLightEnabled = true; // lights on
	if (Application::IsKeyPressed('6'))
		bLightEnabled = false; // lights off
	static float asd = 0;
	asd += (float)dt;
//	camera.position = Vector3(0, 1 + cosf(asd), 1);
	/*if (Application::IsKeyPressed('I'))
		camera.position.z -= 1 * dt;
	if (Application::IsKeyPressed('K'))
		camera.position.z += 1 * dt;
	if (Application::IsKeyPressed('J'))
		camera.position.x -= 1 * dt;
	if (Application::IsKeyPressed('L'))
		camera.position.x += 1 * dt;*/

	//Vector3 right(1, 0, 0);
	//Vector3 dir = camera.up.Cross(right);
	//dir.Normalize();
	//camera.target = camera.position + dir * camera.fDistance;

	//camera.up = right.Cross(dir);
	fps = (float)(1.f / dt);
}

void SceneBase::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureArray[0] <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	float accum = 0;
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(accum, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
		accum += (float)fontWidth[(unsigned)text[i]] / 64;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureArray[0] <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	float accum = 0;
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(accum + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);

		accum += (float)fontWidth[(unsigned)text[i]] / 64;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderMesh(Mesh *mesh, bool enableLight, float alpha)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	if (m_renderPass == RENDER_PASS_PRE)
	{
		Mtx44 lightDepthMVP = m_lightDepthProj *
			m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1,
			GL_FALSE, &lightDepthMVP.a[0]);
		//mesh->Render();
		for (int i = 0; i < MAX_TEXTURES; ++i)
		{
			if (mesh->textureArray[i] > 0)
			{
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED
					+ i], 1);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE + i], i);
			}
			else
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED
					+ i], 0);
		}
		mesh->Render();
		return;
	}
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		Mtx44 lightDepthMVP = m_lightDepthProj *
			m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1,
			GL_FALSE, &lightDepthMVP.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);

	}
	glUniform1f(m_parameters[U_ALPHA], alpha);
	mesh->Render();
	/*if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}*/
}

void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool SceneBase::Handle(Message* message)
{
	return false;
}

void SceneBase::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
