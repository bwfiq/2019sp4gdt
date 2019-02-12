#include "UIManager.h"
#include "UIBase.h"
#include "GL/glew.h"
#include "SceneBase.h"

void UIManager::Init()
{
}

void UIManager::Update(float dt)
{
}

void UIManager::Render(SceneBase * scene)
{
	/*Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	scene->projectionStack.PushMatrix();
	scene->projectionStack.LoadMatrix(ortho);
	scene->viewStack.PushMatrix();
	scene->viewStack.LoadIdentity();
	scene->modelStack.PushMatrix();
	scene->modelStack.LoadIdentity();
	int count = 0;
	for (auto UI : ui_list)
	{
		count++;
	scene->modelStack.PushMatrix();
		scene->modelStack.Translate(UI->pos.x, UI->pos.y, UI->pos.z + (count << 5));
		scene->modelStack.Scale(UI->scale.x, UI->scale.y, 1);
		for (auto UImesh : UI->meshes)
			r
		
	scene->modelStack.PopMatrix();
	}
	

	scene->modelStack.PopMatrix();
	scene->viewStack.PopMatrix();
	scene->projectionStack.PopMatrix();*/
}

bool UIManager::AddUI(UIBase * ui)
{
	return false;
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
	if (mesh->textureID > 0)
	{
		glUniform1i(scene->m_parameters[SceneBase::U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(scene->m_parameters[SceneBase::U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(scene->m_parameters[SceneBase::U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
