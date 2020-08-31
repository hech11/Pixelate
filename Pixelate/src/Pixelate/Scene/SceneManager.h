#pragma once

#include "Pixelate/Scene/Scene.h"

namespace Pixelate {

	class SceneManager {

		public :

			static void Shutdown();

			static Ref<Scene> LoadScene(const std::string& filepath);

			static Ref<Scene> LoadRegisteredScene(const std::string& filepath);
			static Ref<Scene> LoadRegisteredScene(unsigned int index);

			static Ref<Scene> GenerateDefaultScene();
			static Ref<Scene>& GetActiveScene();
			static std::string& GetActiveSceneFilepath();

			static void QueueLoadedScene(const std::string& filepath);
			static void LoadAnyQueuedScenes(Ref<Scene>& editorScene, bool* loadedANewScene);

			static void SetPlayMode(bool playMode);

			static void SaveScene(const Ref<Scene>& scene, const std::string& filepath);
			static void RegisterScene(const std::string& filepath);
			static void SaveCurrentScene(const std::string& filepath);


			// these are temp -- this stores the current scene before play mode so after 
			// the user stops play mode the stored scene is used -- similar to how unity or unreal manage their scenes
			static void StoreSceneBeforePlay();
			static Ref<Scene> LoadStoredSceneAfterRuntimeStop();
			

			static std::vector<std::string>& const GetAllProjectScenesFilepath();

	};

}