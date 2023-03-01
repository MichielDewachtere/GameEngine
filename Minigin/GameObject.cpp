#include "GameObject.h"
#include "ResourceManager.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update() {}

void dae::GameObject::Render() const
{
	//const auto& pos = m_transform.GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	for (const auto& pComponent : m_ComponentPtrs)
	{
		if (pComponent->CanRender()) 
			pComponent->Render();
	}
}

//void dae::GameObject::SetTexture(const std::string& filename)
//{
//	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
//}

//void dae::GameObject::SetPosition(float x, float y)
//{
//	//m_transform.SetPosition(x, y, 0.0f);
//}