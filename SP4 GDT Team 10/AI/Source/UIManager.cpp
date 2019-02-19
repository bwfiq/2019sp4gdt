#include "UIManager.h"
#include "UIBase.h"
#include "GL/glew.h"
#include "SceneBase.h"
#include "Application.h"
#include "SceneData.h"
#include "MouseController.h"
#include "Application.h"
//#include <list>

static const int fontWidth[] = { 0,26,26,26,26,26,26,26,26,26,26,26,26,0,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,12,17,21,26,26,37,35,11,16,16,26,26,13,16,13,20,26,26,26,26,26,26,26,26,26,26,14,14,26,26,26,24,46,30,28,28,32,25,24,33,32,13,17,27,22,44,34,34,27,35,28,24,25,33,30,46,27,25,24,16,20,16,26,26,15,25,27,22,27,26,16,24,27,12,12,24,12,42,27,27,27,27,18,20,17,27,23,37,23,24,21,16,24,16,26,26,26,26,13,16,22,36,26,26,21,54,24,18,45,26,24,26,26,13,13,22,22,26,26,47,23,37,20,18,44,26,21,25,12,17,26,26,26,26,26,26,20,43,21,27,26,16,26,20,18,26,17,17,15,29,30,13,16,13,22,27,33,35,35,24,30,30,30,30,30,30,40,28,25,25,25,25,13,13,13,13,32,34,34,34,34,34,34,26,35,33,33,33,33,25,27,27,25,25,25,25,25,25,40,22,26,26,26,26,12,12,12,12,27,27,27,27,27,27,27,26,28,27,27,27,27,24,27,24 };


void UIManager::Init()
{
	
}

void UIManager::Update(float dt)
{
	bool prevMUIBool = MouseController::GetInstance()->IsMouseOnUI();
	MouseController::GetInstance()->SetMouseOnUI(false);
	for (const auto& it : ui_list)
	{
		it.second->Update(dt);
		it.second->bIsMouseHovered = it.second->IsMouseHovered();
		if (it.second->bIsMouseHovered)
			MouseController::GetInstance()->SetMouseOnUI(true);
	}
	/*if (prevMUIBool != MouseController::GetInstance()->IsMouseOnUI())
	{
		Application::GetInstance().SetMouseVisiblity(MouseController::GetInstance()->IsMouseOnUI());
	}*/ // makes cursor visible only while cursor is over ui
	std::map<std::string, UIBase*>::iterator it = ui_list.begin();
	while (it != ui_list.end())
	{
		if (it->second->bIsDone)
		{
			std::string itName = it->first;
			RemoveUI(itName);
			it = ui_list.begin();
		}
		else
		{
			++it;
		}
	}
}

void UIManager::Render(SceneBase * scene)
{
	Mtx44 ortho;
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() * 0.5f;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() * 0.5f;
	//ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	ortho.SetToOrtho(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);

	Mtx44 projection;
	projection.SetToOrtho(0, scene->m_worldWidth, 0, scene->m_worldHeight, -10, 10);

	scene->projectionStack.PushMatrix();
	scene->projectionStack.LoadMatrix(ortho);
	scene->viewStack.PushMatrix();
	scene->viewStack.LoadIdentity();
	scene->modelStack.PushMatrix();
	scene->modelStack.LoadIdentity();
	float count = 0;
	for (auto it : ui_list)
	{
		UIBase* UI = it.second;
		count += 0.01f;
		if (!UI->bActive) continue;
		scene->modelStack.PushMatrix();
			float count2 = 0;
			Vector3 UI_pos(	(UI->pos.x - 0.5f) * halfWindowWidth * 2,
							(UI->pos.y - 0.5f) * halfWindowHeight * 2,
							 UI->pos.z + count);
			scene->modelStack.Translate(UI_pos.x
				, UI_pos.y
				, UI_pos.z);
			//scene->modelStack.Scale(UI->scale.x * halfWindowWidth, UI->scale.y * halfWindowHeight, 1); dont remove dis comment lul
			scene->modelStack.Scale(UI->scale.x, UI->scale.y, 1);
			//scene->modelStack.Translate(-UI->anchorPoint.x + 0.5f, -UI->anchorPoint.y + 0.5f, 0);
			for (auto UIC : UI->uiComponents_list)
			{
				if (!UIC.bActive) continue;
				Vector3 UIC_Pos(UIC.pos.x - UI->anchorPoint.x + (UIC.anchorPoint.x - 0.5f) * -Math::Min(UIC.scale.x, 1.f)
					, UIC.pos.y - UI->anchorPoint.y + (UIC.anchorPoint.y - 0.5f) * -Math::Min(UIC.scale.y, 1.f)
					, UIC.pos.z + count2);
				count2 += 0.01f;
				scene->modelStack.PushMatrix();
				scene->modelStack.Translate(UIC_Pos.x
					, UIC_Pos.y
					, UIC_Pos.z);
				scene->modelStack.Rotate(UIC.fRotation,0,0,1);
				scene->modelStack.Scale(UIC.scale.x, UIC.scale.y, 1);
				if (UIC.mesh != NULL)
					rendermesh(scene, UIC.mesh, false, UIC.alpha);

				if (!UIC.text.empty())
				{
					count2 += 0.01f;
					rendertext(scene, SceneData::GetInstance()->GetMesh("gungsuh"), UIC.text, UIC.textColor, false, UI_pos + (UIC_Pos + Vector3(-1,0,0)) * (UI->scale.x * 0.5f)
						, UIC.textSize,  UIC.alpha);
				}
				scene->modelStack.PopMatrix();
			}
		scene->modelStack.PopMatrix();
	}
	

	scene->modelStack.PopMatrix();
	scene->viewStack.PopMatrix();
	scene->projectionStack.PopMatrix();
}

bool UIManager::AddUI(const std::string& uiName, UIBase * ui)
{
	//ui_list.push_back(ui);
	if (ui == nullptr || ui == NULL)
		return false;

	RemoveUI(uiName);

	ui_list[uiName] = ui;
	ui->uiName = uiName;
	return true;
}

UIBase * UIManager::GetUI(const std::string & uiName)
{
	if (ui_list.count(uiName) != 0)
		return ui_list[uiName];

	return NULL;
}

UIManager::UIManager()
{

}

UIManager::~UIManager()
{
	while (!ui_list.empty()) {
		std::string uiName = ui_list.begin()->first;
		delete ui_list.begin()->second;
		ui_list.erase(uiName);
	}
}

void UIManager::rendermesh(SceneBase* scene,Mesh * mesh,bool bLightEnabled, float alpha)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = scene->projectionStack.Top() * scene->viewStack.Top() * scene->modelStack.Top();
	glUniformMatrix4fv(scene->m_parameters[SceneBase::U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (bLightEnabled)
	{
		glUniform1i(scene->m_parameters[SceneBase::U_LIGHTENABLED], 1);
		modelView = scene->viewStack.Top() * scene->modelStack.Top();
		glUniformMatrix4fv(scene->m_parameters[SceneBase::U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(scene->m_parameters[SceneBase::U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(scene->m_parameters[SceneBase::U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(scene->m_parameters[SceneBase::U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(scene->m_parameters[SceneBase::U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(scene->m_parameters[SceneBase::U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(scene->m_parameters[SceneBase::U_LIGHTENABLED], 0);
	}

	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(scene->m_parameters[SceneBase::U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(scene->m_parameters[SceneBase::U_COLOR_TEXTURE + i], i);
		}
		else
			glUniform1i(scene->m_parameters[SceneBase::U_COLOR_TEXTURE_ENABLED + i], 0);

	}
	glUniform1f(scene->m_parameters[SceneBase::U_ALPHA], alpha);
	mesh->Render();
	//if (mesh->textureID > 0)
	//{
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}
}

void UIManager::rendertext(SceneBase * scene, Mesh * mesh, std::string text, Color color, bool bLightEnabled, Vector3 pos, float size, float alpha)
{
	if (!mesh || mesh->textureArray[0] <= 0)
		return;
	glDisable(GL_DEPTH_TEST);
	glUniform1i(scene->m_parameters[SceneBase::U_TEXT_ENABLED], 1);
	glUniform3fv(scene->m_parameters[SceneBase::U_TEXT_COLOR], 1, &color.r);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() * 0.5f;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() * 0.5f;
	Mtx44 ortho;
	ortho.SetToOrtho(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);

	scene->projectionStack.PushMatrix();
	scene->projectionStack.LoadMatrix(ortho);
	scene->viewStack.PushMatrix();
	scene->viewStack.LoadIdentity();
	scene->modelStack.PushMatrix();
	scene->modelStack.LoadIdentity();
	scene->modelStack.Translate(pos.x, pos.y, pos.z);
	scene->modelStack.Scale(size, size, 1);
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	float accum = 0;
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(accum + 0.5f, 0, 0); //1.0f is the spacing of each character, you may change this value
		MVP = scene->projectionStack.Top() * scene->viewStack.Top() * scene->modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(scene->m_parameters[SceneBase::U_MVP], 1, GL_FALSE, &MVP.a[0]);
		if (bLightEnabled)
		{
			glUniform1i(scene->m_parameters[SceneBase::U_LIGHTENABLED], 1);
			modelView = scene->viewStack.Top() * scene->modelStack.Top();
			glUniformMatrix4fv(scene->m_parameters[SceneBase::U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
			modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
			glUniformMatrix4fv(scene->m_parameters[SceneBase::U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

			//load material
			glUniform3fv(scene->m_parameters[SceneBase::U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
			glUniform3fv(scene->m_parameters[SceneBase::U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
			glUniform3fv(scene->m_parameters[SceneBase::U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
			glUniform1f(scene->m_parameters[SceneBase::U_MATERIAL_SHININESS], mesh->material.kShininess);
		}
		else
		{
			glUniform1i(scene->m_parameters[SceneBase::U_LIGHTENABLED], 0);
		}

		for (int i = 0; i < MAX_TEXTURES; ++i)
		{
			if (mesh->textureArray[i] > 0)
			{
				glUniform1i(scene->m_parameters[SceneBase::U_COLOR_TEXTURE_ENABLED + i], 1);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
				glUniform1i(scene->m_parameters[SceneBase::U_COLOR_TEXTURE + i], i);
			}
			else
				glUniform1i(scene->m_parameters[SceneBase::U_COLOR_TEXTURE_ENABLED + i], 0);

		}
		glUniform1f(scene->m_parameters[SceneBase::U_ALPHA], alpha);
		mesh->Render((unsigned)text[i] * 6, 6);

		accum += 32.f / 64.f;//(float)fontWidth[(unsigned)text[i]] / 64.f;
	}
	//glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(scene->m_parameters[SceneBase::U_TEXT_ENABLED], 0);
	//if (mesh->textureID > 0)
	//{
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}
	scene->modelStack.PopMatrix();
	scene->viewStack.PopMatrix();
	scene->projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

bool UIManager::RemoveUI(const std::string & uiName)
{
	UIBase* currUI = GetUI(uiName);
	if (currUI != nullptr && currUI != NULL)
	{
		delete currUI;
		ui_list.erase(uiName);
		return true;
	}
	return false;
}
