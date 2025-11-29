#include "../include/ui_transformation_system.h"
#include "../include/ui_component.h"
#include <glm/gtc/quaternion.hpp>



TheEngine::ECS::ECSEngine* UITransformationSystem::m_ecsEngine = nullptr;

void UITransformationSystem::setECSEngine(TheEngine::ECS::ECSEngine& ecsEngine)
{
	m_ecsEngine = &ecsEngine;
}





void UITransformationSystem::updateLocalTransformation()
{

	TheEngine::ECS::Query query = m_ecsEngine->getQuery<UI::UITransformComponent>();


	for (auto& chunkArrayView : query.getChunkArrayViews())
	{
		const size_t count = chunkArrayView.getCount();

		for (size_t i = 0; i < count; ++i)
		{

			UI::UITransformComponent* UITransformComponentPtr = chunkArrayView.getComponentArray<UI::UITransformComponent>();

			if (UITransformComponentPtr == nullptr ||false /*bool flag check if dirty*/)
			{
				continue;
			}


			glm::mat4 rotationMatrix = glm::mat4_cast(UITransformComponentPtr->rotation);


			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), UITransformComponentPtr->scale);

			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), UITransformComponentPtr->position);



			UITransformComponentPtr->localTransfrom = translationMatrix * rotationMatrix * scaleMatrix;

		}



	}


}
