#pragma once

#include "Pixelate/Editor/EditorPanel.h"
#include <array>
namespace Pixelate {


	class EditorTextureInspector : public EditorPanel {

		public:

			void OnImguiRender() override;
			void OnEvent(Event& e) override;


			void SetTextureContext(const Ref<Texture>& context) { m_CurrentContext = context; }
		private :

			void ApplyChanges(const Ref<Texture>& texture);
			bool OnMouseScrolled(MouseScrolledEvent& e);
		private :
			float m_ZoomSize = 1.0f;
			TextureProperties m_CurrentTextureProps;
			Ref<Texture> m_CurrentContext;

			std::map<std::string, TextureProperties::Format> m_FormatResolver = 
			{ 
				{"None", TextureProperties::Format::None }, 
				{"RGB", TextureProperties::Format::RGB },
				{"RGBA", TextureProperties::Format::RGBA }
			};
			std::map<std::string, TextureProperties::Filter> m_FilterResolver =
			{
				{"None", TextureProperties::Filter::None },
				{"Nearest", TextureProperties::Filter::Nearest },
				{"Linear", TextureProperties::Filter::Linear }
			};

			std::map<std::string, TextureProperties::Wrap> m_WrapResolver =
			{
				{"None", TextureProperties::Wrap::None },
				{"Repeat", TextureProperties::Wrap::Repeat},
				{"Clamp_To_Edge", TextureProperties::Wrap::Clamp_To_Edge }
			};
			

	};
}