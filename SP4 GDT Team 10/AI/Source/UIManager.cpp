#include "UIManager.h"
#include "UIBase.h"
#include "GL/glew.h"
#include "SceneBase.h"
#include "Application.h"

void UIManager::Init()
{
}

void UIManager::Update(float dt)
{
	for (auto UI : ui_list)
	{
		UI->Update(dt);
	}
}

void UIManager::Render(SceneBase * scene)
{
	Mtx44 ortho;
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() * 0.5f;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() * 0.5f;
	//ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	ortho.SetToOrtho(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);

	scene->projectionStack.PushMatrix();
	scene->projectionStack.LoadMatrix(ortho);
	scene->viewStack.PushMatrix();
	scene->viewStack.LoadIdentity();
	scene->modelStack.PushMatrix();
	scene->modelStack.LoadIdentity();
	float count = 0;
	for (auto UI : ui_list)
	{
		count += 0.01f;
		scene->modelStack.PushMatrix();
			scene->modelStack.Translate((UI->pos.x - 0.5f) * halfWindowWidth * 2 //+ UI->scale.x - (UI->anchorPoint.x + 0.5f) * UI->scale.x
				, (UI->pos.y - 0.5f) * halfWindowHeight * 2 //+ UI->scale.y - (UI->anchorPoint.y + 0.5f) * UI->scale.y
				, UI->pos.z + count);
			//scene->modelStack.Scale(UI->scale.x * halfWindowWidth, UI->scale.y * halfWindowHeight, 1); dont remove dis comment lul
			scene->modelStack.Scale(UI->scale.x, UI->scale.y, 1);
			//scene->modelStack.Translate(-UI->anchorPoint.x + 0.5f, -UI->anchorPoint.y + 0.5f, 0);
			float count2 = 0;
			for (auto UIC : UI->uiComponents_list)
			{
				count2 += 0.01f;
				scene->modelStack.PushMatrix();
				scene->modelStack.Translate(UIC.pos.x - UI->anchorPoint.x - UIC.anchorPoint.x + (UIC.anchorPoint.x + (UIC.anchorPoint.x - 0.5f) * -Math::Min(UIC.scale.x, 1.f))
					, UIC.pos.y - UI->anchorPoint.y - UIC.anchorPoint.y + (UIC.anchorPoint.y + (UIC.anchorPoint.y - 0.5f) * -Math::Min(UIC.scale.y, 1.f))
					, UIC.pos.z + count2);
				scene->modelStack.Scale(UIC.scale.x, UIC.scale.y, 1);
				rendermesh(scene, UIC.mesh, false);
				scene->modelStack.PopMatrix();
			}
		scene->modelStack.PopMatrix();
	}
	

	scene->modelStack.PopMatrix();
	scene->viewStack.PopMatrix();
	scene->projectionStack.PopMatrix();
}

bool UIManager::AddUI(UIBase * ui)
{
	ui_list.push_back(ui);
	return true;
}

UIManager::UIManager()
{

}

UIManager::~UIManager()
{
	while (!ui_list.empty()) {
		delete ui_list.back();
		ui_list.pop_back();
	}
}

void UIManager::rendermesh(SceneBase* scene,Mesh * mesh,bool bLightEnabled)
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
	glUniform1f(scene->m_parameters[SceneBase::U_ALPHA], 1.f);
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
