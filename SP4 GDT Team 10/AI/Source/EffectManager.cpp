#include "EffectManager.h"
#include "EffectBase.h"
#include "GL/glew.h"
#include "SceneBase.h"
#include "Application.h"
#include "EffectTrail.h"
#include "EffectHand.h"

void EffectManager::Init()
{
}

void EffectManager::Update(float dt)
{
	for (auto Effect : effect_list)
	{
		Effect->Update(dt);
	}
}

void EffectManager::Render(SceneBase * scene)
{
	float count = 0;
	for (auto Effect : effect_list)
	{
		count += 0.01f;
		scene->modelStack.PushMatrix();
		EffectTrail* EffTrail = dynamic_cast<EffectTrail*>(Effect);
		EffectHand* EffHand = dynamic_cast<EffectHand*>(Effect);
		if (EffTrail)
		{
			int i = 0;
			for (std::list<Vector3>::iterator it = EffTrail->trailPoints.begin(); it!=EffTrail->trailPoints.end(); ++it)
			{
				if (i >= EffTrail->iTotalPoints - 1) break;
				Vector3 pointA = *(it++);
				Vector3 pointB = *(it);
				it--;
				Vector3 AtoB = (pointB - pointA);
				if (AtoB.IsZero()) 
				{
					++i;
					continue;
				}
				float AtoB_Length = AtoB.Length();
				AtoB.Normalize();
				float lineWidth = Math::Max((float)i / (float)EffTrail->iTotalPoints,0.f);
				glLineWidth(lineWidth * EffTrail->fLineThickness);
				float angle_y = asin(AtoB.y);

				// Calculate the length of the laser when it is projected onto the x-z plane
				float theDirection_LengthInXYPlane = (float)(cos(angle_y));

				// Calculate the angle of the laser with respect from the x-axis (1st Quadrant)
				float angle_x = acos(AtoB.x / theDirection_LengthInXYPlane);

				// If the laser is going to the 3rd and 4th Quadrant, 
				// then we need to calculate the angle from x-axis in the clockwise direction
				if (AtoB.z >= 0)
					angle_x = (2 * Math::PI) - angle_x;

				// Calculate the angle of the laser from the z axis.
				float angle_z = acos(AtoB.z / theDirection_LengthInXYPlane);

				scene->modelStack.PushMatrix();
				scene->modelStack.Translate(pointA.x, pointA.y + 0.001f, pointA.z);
				scene->modelStack.Rotate(180 / Math::PI * angle_x, 0.0f, 1.0f, 0.0f);
			
				scene->modelStack.Rotate(180 / Math::PI * angle_y, 0.0f, 0.0f, 1.0f);
			
				scene->modelStack.Scale(AtoB_Length,1,1);
				rendermesh(scene, Effect);

				scene->modelStack.PopMatrix();
				++i;
			}
			glLineWidth(1.f);
		}
		else if (EffHand)
		{
			scene->modelStack.Translate(Effect->pos.x, Effect->pos.y, Effect->pos.z);
			if (!Effect->rotation.IsZero())
			{
				scene->modelStack.Rotate(Effect->rotation.x, 1, 0, 0);
				scene->modelStack.Rotate(Effect->rotation.y, 0, 1, 0);
				scene->modelStack.Rotate(Effect->rotation.z, 0, 0, 1);

			}
			scene->modelStack.Scale(Effect->scale.x, Effect->scale.y, Effect->scale.z);
			//glDepthFunc(GL_ALWAYS);
			rendermesh(scene, Effect);
			//glDepthFunc(GL_LESS);
		}
		else
		{
			scene->modelStack.Translate(Effect->pos.x, Effect->pos.y, Effect->pos.z);
			if (!Effect->rotation.IsZero())
			{
				scene->modelStack.Rotate(Effect->rotation.x, 1, 0, 0);
				scene->modelStack.Rotate(Effect->rotation.y, 0, 1, 0);
				scene->modelStack.Rotate(Effect->rotation.z, 0, 0, 1);

			}
			scene->modelStack.Scale(Effect->scale.x, Effect->scale.y, Effect->scale.z);
			rendermesh(scene, Effect);
		}
		scene->modelStack.PopMatrix();
	}
}

bool EffectManager::AddEffect(EffectBase* effect)
{
	effect_list.push_back(effect);
	return true;
}

EffectManager::EffectManager()
{

}

EffectManager::~EffectManager()
{
	while (!effect_list.empty()) {
		delete effect_list.back();
		effect_list.pop_back();
	}
}

void EffectManager::rendermesh(SceneBase* scene,EffectBase* effect)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = scene->projectionStack.Top() * scene->viewStack.Top() * scene->modelStack.Top();
	glUniformMatrix4fv(scene->m_parameters[SceneBase::U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (effect->bLightEnabled)
	{
		glUniform1i(scene->m_parameters[SceneBase::U_LIGHTENABLED], 1);
		modelView = scene->viewStack.Top() * scene->modelStack.Top();
		glUniformMatrix4fv(scene->m_parameters[SceneBase::U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(scene->m_parameters[SceneBase::U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(scene->m_parameters[SceneBase::U_MATERIAL_AMBIENT], 1, &effect->mesh->material.kAmbient.r);
		glUniform3fv(scene->m_parameters[SceneBase::U_MATERIAL_DIFFUSE], 1, &effect->mesh->material.kDiffuse.r);
		glUniform3fv(scene->m_parameters[SceneBase::U_MATERIAL_SPECULAR], 1, &effect->mesh->material.kSpecular.r);
		glUniform1f(scene->m_parameters[SceneBase::U_MATERIAL_SHININESS], effect->mesh->material.kShininess);
	}
	else
	{
		glUniform1i(scene->m_parameters[SceneBase::U_LIGHTENABLED], 0);
	}
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (effect->mesh->textureArray[i] > 0)
		{
			glUniform1i(scene->m_parameters[SceneBase::U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, effect->mesh->textureArray[i]);
			glUniform1i(scene->m_parameters[SceneBase::U_COLOR_TEXTURE + i], i);
		}
		else
			glUniform1i(scene->m_parameters[SceneBase::U_COLOR_TEXTURE_ENABLED + i], 0);

	}
	glUniform1f(scene->m_parameters[SceneBase::U_ALPHA], 1.f);
	effect->mesh->Render();
	//if (effect->mesh->textureID > 0)
	//{
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}
}
