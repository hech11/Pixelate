#include "PXpch.h"
#include "EditorTextureInspector.h"

#include <imgui.h>


namespace Pixelate {


	static std::string GetFormatID(const TextureProperties& props) {
		switch (props.TexFormat)
		{
			case TextureProperties::Format::None:
				return "None";
			case TextureProperties::Format::RGB:
				return "RGB";
			case TextureProperties::Format::RGBA:
				return "RGBA";
			default:
				return "Not listed!";
		}

	}


	static std::string GetFilterID(const TextureProperties& props) {
		switch (props.TexFilter)
		{
			case TextureProperties::Filter::None:
				return "None";
			case TextureProperties::Filter::Linear:
				return "Linear";
			case TextureProperties::Filter::Nearest:
				return "Nearest";
			default:
				return "Not listed!";
		}
	}

	static std::string GetWrapID(const TextureProperties& props) {
		switch (props.TexWrap)
		{
			case TextureProperties::Wrap::None:
				return "None";
			case TextureProperties::Wrap::Repeat:
				return "Repeat";
			case TextureProperties::Wrap::Clamp_To_Edge:
				return "Clamp To Edge";
			default:
				return "Not listed!";
		}
	}

	static TextureProperties::Filter ConvertStringToFilter(const std::string& filter) {

	}


	static TextureProperties::Format ConvertStringToFormat(const std::string & format) {

	}

	static TextureProperties::Wrap ConvertStringToWrap(const std::string& wrap) {
		
	}

	template<size_t size>
	static void DisplayTextureProp(std::string& currentItem, std::array<std::string, size>& itemArray, const char* id) {

		if (ImGui::BeginCombo(id, currentItem.c_str())) // The second parameter is the label previewed before opening the combo.
		{
			for (std::string& item: itemArray) {
				bool is_selected = (currentItem == item); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(item.c_str(), is_selected))
					currentItem = item;

				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();


		}
	}


	void EditorTextureInspector::OnImguiRender() {
		if (m_IsPanelOpen) {
			ImGui::Begin("Texture Inspector", &m_IsPanelOpen);
			if (m_CurrentContext) {

				const auto& texture = m_CurrentContext;

				ImVec2 textureSize = { texture->GetWidth() * m_ZoomSize, texture->GetHeight() * m_ZoomSize };
				ImVec2 texturePosition = { (ImGui::GetWindowSize().x - textureSize.x) * 0.5f, (ImGui::GetWindowSize().y - textureSize.y) * 0.5f };
				ImGui::SetCursorPos(texturePosition);
				ImGui::Image((ImTextureID)texture->GetHandleID(), textureSize, { 0, 1 }, { 1, 0 });




				ImGui::Separator();

				m_CurrentTextureProps = texture->GetTextureProps();

				static std::array<std::string, 2> ItemFormats = { "RGB", "RGBA" };
				static std::array<std::string, 2> ItemFilters = { "Nearest", "Linear" };
				static std::array<std::string, 2> ItemWraps = { "Clamp_To_Edge", "Repeat" };

				static std::string ItemFormat = GetFormatID(m_CurrentTextureProps);
				static std::string ItemFilter = GetFilterID(m_CurrentTextureProps);
				static std::string ItemWrap = GetWrapID(m_CurrentTextureProps);


				ImGui::Text(texture->GetFilepath().c_str());
				ImGui::Text("Format: "); ImGui::SameLine(); DisplayTextureProp(ItemFormat, ItemFormats, "##Format");
				ImGui::Text("Filter: "); ImGui::SameLine(); DisplayTextureProp(ItemFilter, ItemFilters, "##Filters");
				ImGui::Text("Wrapping mode: "); ImGui::SameLine(); DisplayTextureProp(ItemWrap, ItemWraps, "##Wraps");

				m_CurrentTextureProps.TexFilter = m_FilterResolver[ItemFilter];
				m_CurrentTextureProps.TexFormat = m_FormatResolver[ItemFormat];
				m_CurrentTextureProps.TexWrap = m_WrapResolver[ItemWrap];

				if (ImGui::Button("Apply changes")) {
					ApplyChanges(texture);
				}

			}
			ImGui::End();
		}

	}

	void EditorTextureInspector::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(PX_BIND_EVENT_FNC(EditorTextureInspector::OnMouseScrolled));
	}

	void EditorTextureInspector::ApplyChanges(const Ref<Texture>& texture)
	{
		texture->SetProperties(m_CurrentTextureProps);
		texture->SetData(texture->GetFilepath().c_str());
	}

	bool EditorTextureInspector::OnMouseScrolled(MouseScrolledEvent& e)
	{
		
		m_ZoomSize += e.GetYScroll() * 0.25f;
		m_ZoomSize = std::max(m_ZoomSize, 0.25f);


		return false;
	}

}