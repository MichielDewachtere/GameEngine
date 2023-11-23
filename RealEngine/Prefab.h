#ifndef PREFAB_H
#define PREFAB_H


namespace real
{
	class GameObject;
	class Scene;

	class Prefab
	{
	public:
		explicit Prefab(Scene& scene);
		explicit Prefab(GameObject& gameObject);
		virtual ~Prefab() = default;

		Prefab(const Prefab& other) = delete;
		Prefab& operator=(const Prefab& rhs) = delete;
		Prefab(Prefab&& other) = delete;
		Prefab& operator=(Prefab&& rhs) = delete;

		GameObject& GetGameObject() const { return *m_pGameObject; }

	protected:
		GameObject* m_pGameObject;

		//virtual void InitComponents() = 0;
	};
}

#endif // PREFAB_H