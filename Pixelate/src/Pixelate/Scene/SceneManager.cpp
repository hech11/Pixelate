#include "PXpch.h"
#include "SceneManager.h"

#include "SceneSerialization.h"
#include "Pixelate/Core/Application.h"

#include "Components.h"
#include "Entity.h"



namespace Pixelate {

	struct SceneManagerData {
			
		Ref<Scene> ActiveScene, ActivePlayingScene;
		std::string ActiveSceneFilepath;
		std::string SceneFilepathToBeQueued;
		std::vector<std::string> AllScenesInProject;

		bool PlayMode = false;
		bool QueueSceneLoading = false;
		//bool HasCurrentSceneBeenSaved = false;
	};

	static SceneManagerData s_SceneManagerData;

	
	void SceneManager::Shutdown() {
		if (s_SceneManagerData.PlayMode)
			s_SceneManagerData.ActiveScene->OnRuntimeStop();
	}

	Ref<Scene> SceneManager::LoadScene(const std::string& filepath) {

		auto& scene = s_SceneManagerData.ActiveScene;
		auto& app = Application::GetApp();

		if (s_SceneManagerData.PlayMode) {
			scene->OnRuntimeStop();
			scene  = SceneSerialization::Deserialize(filepath);
			scene->OnRuntimeStart();
		} else
			scene = SceneSerialization::Deserialize(filepath);

		if (scene == nullptr) {
			scene = GenerateDefaultScene();
		}
		s_SceneManagerData.ActiveSceneFilepath = filepath;
		s_SceneManagerData.ActiveScene = scene;
		app.GetWindow().SetTitle("Pixelate-Editor | " + scene->GetName());
		
		return scene;

	}

	void SceneManager::RegisterScene(const std::string& filepath) {
		for (unsigned int i = 0; i < s_SceneManagerData.AllScenesInProject.size(); i++) {
			if (s_SceneManagerData.AllScenesInProject[i] == filepath)
				return;
		}
		s_SceneManagerData.AllScenesInProject.emplace_back(filepath);
	}

	Ref<Scene> SceneManager::LoadRegisteredScene(const std::string& filepath) {
		for (auto fp : s_SceneManagerData.AllScenesInProject) {
			if(fp == filepath)
				return LoadScene(filepath);
		}
		return GenerateDefaultScene();
	}

	Ref<Scene> SceneManager::LoadRegisteredScene(unsigned int index) {
		if (s_SceneManagerData.AllScenesInProject.size() < index) {
			PX_CORE_ERROR("[SceneManager]: index '%d' is not in the managers data structure!\n", index);
			return s_SceneManagerData.ActiveScene;
		}
		return LoadScene(s_SceneManagerData.AllScenesInProject[index]);
	}

	Ref<Scene> SceneManager::GenerateDefaultScene()
	{
		Ref<Scene> temp = CreateRef<Scene>();

		auto camEntity = temp->CreateEntity("Camera");
		camEntity.AddComponent<CameraComponent>();
		camEntity.AddComponent<AudioListenerComponent>();

		
		s_SceneManagerData.ActiveScene = temp;

		Application::GetApp().GetWindow().SetTitle("Pixelate-Editor | " + temp->GetName());

		return temp;

	}

	Ref<Scene>& SceneManager::GetActiveScene() {
		return s_SceneManagerData.ActiveScene;
	}

	std::string& SceneManager::GetActiveSceneFilepath() {
		return s_SceneManagerData.ActiveSceneFilepath;
	}

	void SceneManager::QueueLoadedScene(const std::string& filepath) {
		s_SceneManagerData.QueueSceneLoading = true;
		s_SceneManagerData.SceneFilepathToBeQueued = filepath;

	}

	void SceneManager::LoadAnyQueuedScenes(Ref<Scene>& editorScene, bool* out_loadedANewScene) {
		if (s_SceneManagerData.QueueSceneLoading) {
			editorScene = LoadScene(s_SceneManagerData.SceneFilepathToBeQueued);
			*out_loadedANewScene = true;
			s_SceneManagerData.QueueSceneLoading = false;
		}
	}

	void SceneManager::SetPlayMode(bool playMode) {
		s_SceneManagerData.PlayMode = playMode;
	}

	void SceneManager::SaveScene(const Ref<Scene>& scene, const std::string& filepath) {
		SceneSerialization::Serialize(scene, filepath);

		Application::GetApp().GetWindow().SetTitle("Pixelate-Editor | " + scene->GetName());

	}

	void SceneManager::SaveCurrentScene(const std::string& filepath) {
		if(s_SceneManagerData.ActiveScene != nullptr)
			SaveScene(s_SceneManagerData.ActiveScene, filepath);
	}


	void SceneManager::StoreSceneBeforePlay()
	{
		SaveScene(s_SceneManagerData.ActiveScene, "assets/ACTIVESCENE.PXScene");
	}

	Ref<Scene> SceneManager::LoadStoredSceneAfterRuntimeStop()
	{
		return LoadScene("assets/ACTIVESCENE.PXScene");
	}


	std::vector<std::string>& const SceneManager::GetAllProjectScenesFilepath() {
		return s_SceneManagerData.AllScenesInProject;
	}

}