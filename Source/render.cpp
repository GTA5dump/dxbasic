#include "pch.h"
#include "icon_font.h"
#include <functional>

bool g_open_key_pressed = false;
bool g_enter_key_pressed = false;
bool g_back_key_pressed = false;
bool g_up_key_pressed = false;
bool g_down_key_pressed = false;
bool g_left_key_pressed = false;
bool g_right_key_pressed = false;
size_t g_open_delay = 20;
size_t g_back_delay = 20;
size_t g_enter_delay = 20;
size_t g_vertical_delay = 10;
size_t g_horizontal_delay = 10;
int g_max_vis_options = 12;
int g_option_count = 0;
int g_current_option = 0;
void(*g_current_menu)();
const char* g_current_subtitle;
int g_menu_level = 0;
int g_option_array[1000];
const char* m_subtitle_array[1000];
void(*g_menu_array[1000])();
const char* g_info_text = "";
float posx = 25.f;
float posy = 5.f;
float width = 350.f;
ImVec2 g_header_rect_size = { width, 100.f };
ImVec2 g_submenu_rect_size = { width, 37.f };
ImVec2 g_option_rect_size = { width, 35.f };
ImVec2 g_footer_rect_size = { width, 37.f };
ImVec2 g_infobox_rect_size = { width, 40.f };
ImU32 g_header_rect_color = IM_COL32(14, 14, 14, 255);
ImU32 g_subtitle_rect_color = IM_COL32(10, 10, 10, 210);
ImU32 g_option_rect_color = IM_COL32(0, 0, 0, 160);
ImU32 g_footer_rect_color = IM_COL32(10, 10, 10, 210);
ImU32 g_infobox_rect_color = IM_COL32(20, 20, 20, 255);
ImU32 g_header_text_color = IM_COL32(255, 255, 255, 255);
ImU32 g_subtitle_text_color = IM_COL32(255, 255, 255, 255);
ImU32 g_option_text_color = IM_COL32(255, 255, 255, 255);
ImU32 g_footer_text_color = IM_COL32(255, 255, 255, 255);
ImU32 g_infobox_text_color = IM_COL32(255, 255, 255, 255);
ImU32 g_option_selected_rect_color = IM_COL32(255, 255, 255, 255);
ImU32 g_option_selected_text_color = IM_COL32(10, 10, 10, 255);

ImVec2 add(ImVec2 vectorA, ImVec2 vectorB) {
	ImVec2 result;
	result.x = vectorA.x; result.y = vectorA.y;
	result.x += vectorB.x; result.y += vectorB.y;
	return result;
}
void add_rect(ImVec2 pos, ImVec2 size, ImU32 color, ImDrawList* draw_list = ImGui::GetBackgroundDrawList()) {
	auto calcdsize = ImGui::CalcItemSize(ImVec2(size.x, size.y), 0.0f, 0.0f);
	const ImRect rect(ImVec2(pos.x, pos.y), add(ImVec2(pos.x, pos.y), calcdsize));
	draw_list->AddRectFilled(rect.Max, rect.Min, ImGui::GetColorU32(color));
}
void add_text(const char* text, ImVec2 pos, ImU32 color, ImFont* font) {
	auto draw_list = ImGui::GetForegroundDrawList();
	ImGui::PushFont(font);
	draw_list->AddText(pos, color, text);
	ImGui::PopFont();
}
void menulevel_move_menu(void(*menu)(), const char* subtitle_text) {
	g_menu_array[g_menu_level] = g_current_menu;
	g_option_array[g_menu_level] = g_current_option;
	m_subtitle_array[g_menu_level] = g_current_subtitle;
	g_menu_level++;
	g_current_menu = menu;
	g_current_subtitle = subtitle_text;
	g_current_option = 1;
}
void menulevel_back_menu() {
	g_menu_level--;
	g_current_menu = g_menu_array[g_menu_level];
	g_current_option = g_option_array[g_menu_level];
	g_current_subtitle = m_subtitle_array[g_menu_level];
}
void add_menu_header(const char* title) {
	auto headerTextOffset = ImGui::CalcTextSize(title).x;
	auto offset = posx + (width / 2) - headerTextOffset - 25.f;
	add_rect(ImVec2(posx, posy + g_option_rect_size.y), g_header_rect_size, g_header_rect_color);
	add_text(title, ImVec2(posx + offset, posy + g_option_rect_size.y + (g_option_rect_size.y / 2) + 10.f), g_header_text_color, (g_render.m_header_font));
}
void add_menu_subtitle(const char* title) {
	auto option_pos_text = std::to_string(g_current_option) + "/" + std::to_string(g_option_count);
	auto right_align_offset = width - ImGui::CalcTextSize(option_pos_text.c_str()).x - 5.25f;
	add_rect(ImVec2(posx, posy + g_header_rect_size.y + g_option_rect_size.y), g_submenu_rect_size, g_subtitle_rect_color);
	add_text(title, ImVec2(posx + 7.f, posy + g_header_rect_size.y + g_option_rect_size.y + 5), g_subtitle_text_color, (g_render.m_subtitle_font));
	add_text(option_pos_text.c_str(), ImVec2(posx + right_align_offset, posy + g_header_rect_size.y + g_option_rect_size.y + 5.f), g_subtitle_text_color, (g_render.m_subtitle_font));
}
bool add_text_option(const char* option_name, const char* info_text = "") {
	g_option_count++;
	auto offset = 7.f;
	auto on_this = g_current_option == g_option_count ? true : false;
	if (g_current_option <= g_max_vis_options && g_option_count <= g_max_vis_options) {
		add_rect(ImVec2(posx, posy + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * g_option_count), g_option_rect_size, g_option_rect_color);
		if (on_this)
			add_rect(ImVec2(posx, posy + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * g_option_count), g_option_rect_size, g_option_selected_rect_color);
		add_text(option_name, ImVec2(posx + offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * g_option_count), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_font));
	}
	else if (g_option_count > (g_current_option - g_max_vis_options) && g_option_count <= g_current_option) {
		add_rect(ImVec2(posx, posy + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count - (g_current_option - g_max_vis_options))), g_option_rect_size, g_option_rect_color);
		if (on_this)
			add_rect(ImVec2(posx, posy + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count - (g_current_option - g_max_vis_options))), g_option_rect_size, g_option_selected_rect_color);
		add_text(option_name, ImVec2(posx + offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count - (g_current_option - g_max_vis_options))), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_font));
	}
	if (on_this) {
		if (g_info_text != info_text)
			g_info_text = info_text;
		if (g_enter_key_pressed)
			return true;
	}
	return false;
}
bool add_toggle_option(bool* enable_bool, const char* option, const char* info_text = "") {
	add_text_option(option, info_text);
	auto on_this = g_current_option == g_option_count ? true : false;
	auto offset = width - 20.f;
	if (g_current_option <= g_max_vis_options && g_option_count <= g_max_vis_options)
		add_text(*enable_bool ? "g" : "b", ImVec2(posx + offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * g_option_count), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_icon_font));
	else if (g_option_count > g_current_option - g_max_vis_options && g_option_count <= g_current_option)
		add_text(*enable_bool ? "g" : "b", ImVec2(posx + offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count - (g_current_option - g_max_vis_options))), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_icon_font));
	if (on_this && g_enter_key_pressed) {
		*enable_bool = !*enable_bool;
		return true;
	}
	return false;
}
bool add_submenu_option(void(*menu)(), const char* option, const char* subtitle_text, const char* info_text = "") {
	add_text_option(option, info_text);
	auto on_this = g_current_option == g_option_count ? true : false;
	auto offset = width - 20.f;
	if (g_current_option <= g_max_vis_options && g_option_count <= g_max_vis_options)
		add_text("r", ImVec2(posx + offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * g_option_count), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_icon_font));
	else if (g_option_count > g_current_option - g_max_vis_options && g_option_count <= g_current_option)
		add_text("r", ImVec2(posx + offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count - (g_current_option - g_max_vis_options))), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_icon_font));
	if (on_this && g_enter_key_pressed) {
		menulevel_move_menu(menu, subtitle_text);
		return true;
	}
	return false;
}
bool add_editor_option(int& inttochange, int min, int max, int step, const char* option, const char* info_text = "") {
	add_text_option(option, info_text);
	auto on_this = g_current_option == g_option_count ? true : false;
	if (inttochange == -1)
		inttochange = max;
	auto option_text = std::to_string(inttochange);
	auto right_align_offset = width - ImGui::CalcTextSize(option_text.c_str()).x - 4.25f;
	if (g_current_option <= g_max_vis_options && g_option_count <= g_max_vis_options)
		add_text(option_text.c_str(), ImVec2(posx + right_align_offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * g_option_count), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_font));
	else if (g_option_count > g_current_option - g_max_vis_options && g_option_count <= g_current_option) {
		add_text(option_text.c_str(), ImVec2(posx + right_align_offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count - (g_current_option - g_max_vis_options))), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_font));
	}
	if (on_this) {
		if (g_left_key_pressed)
			inttochange >= min ? inttochange -= step : inttochange = max;
		if (g_right_key_pressed)
			inttochange < max ? inttochange += step : inttochange = min;
		return true;
	}
	return false;
}
bool add_editor_option(float& floattochange, float min, float max, float step, const char* option, const char* info_text = "") {
	add_text_option(option, info_text);
	auto on_this = g_current_option == g_option_count ? true : false;
	if (floattochange == -1)
		floattochange = max;
	auto option_text = std::format("{}", floattochange);
	auto right_align_offset = width - ImGui::CalcTextSize(option_text.c_str()).x - 4.25f;
	if (g_current_option <= g_max_vis_options && g_option_count <= g_max_vis_options) {
		add_text(option_text.c_str(), ImVec2(posx + right_align_offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * g_option_count), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_font));
	}
	else if (g_option_count > g_current_option - g_max_vis_options && g_option_count <= g_current_option) {
		add_text(option_text.c_str(), ImVec2(posx + right_align_offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count - (g_current_option - g_max_vis_options))), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_font));
	}
	if (on_this) {
		if (g_left_key_pressed)
			floattochange >= min ? floattochange -= step : floattochange = max;
		if (g_right_key_pressed)
			floattochange < max ? floattochange += step : floattochange = min;
		return true;
	}
	return false;
}
bool add_vector_option(std::vector<const char*> vector, int& position, const char* option, const char* info_text = "") {
	add_text_option(option, info_text);
	auto on_this = g_current_option == g_option_count ? true : false;
	auto min = 0;
	auto max = vector.size() - 1;
	if (position == -1)
		position = max;
	auto right_align_offset = width - ImGui::CalcTextSize(vector[position]).x - 4.25f;
	if (g_current_option <= g_max_vis_options && g_option_count <= g_max_vis_options)
		add_text(vector[position], ImVec2(posx + right_align_offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * g_option_count), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_font));
	else if (g_option_count > g_current_option - g_max_vis_options && g_option_count <= g_current_option)
		add_text(vector[position], ImVec2(posx + right_align_offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count - (g_current_option - g_max_vis_options))), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_font));
	if (on_this) {
		//Right press works, but left press crashes
		if (g_left_key_pressed)
			position >= min ? position-- : position = max;
		if (g_right_key_pressed)
			position < max ? position++ : position = min;
	}
	return false;
}
bool add_vector_option(std::vector<std::string> vector, int& position, const char* option, const char* info_text = "") {
	add_text_option(option, info_text);
	auto on_this = g_current_option == g_option_count ? true : false;
	auto min = 0;
	auto max = vector.size() - 1;
	if (position == -1)
		position = max;
	auto right_align_offset = width - ImGui::CalcTextSize(vector[position].c_str()).x - 4.25f;
	if (g_current_option <= g_max_vis_options && g_option_count <= g_max_vis_options) {
		add_text(vector[position].c_str(), ImVec2(posx + right_align_offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * g_option_count), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_font));
	}
	else if (g_option_count > g_current_option - g_max_vis_options && g_option_count <= g_current_option) {
		add_text(vector[position].c_str(), ImVec2(posx + right_align_offset, posy + 9.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count - (g_current_option - g_max_vis_options))), on_this ? g_option_selected_text_color : g_option_text_color, (g_render.m_font));
	}
	if (on_this) {
		if (g_left_key_pressed)
			position >= min ? position-- : position = max;
		if (g_right_key_pressed)
			position < max ? position++ : position = min;
		return true;
	}
	return false;
}
void add_menu_footer() {
	float arrowOffset = width / 2.f - 10.f;
	float centerArrowOffset = (g_footer_rect_size.y / 2) - 10.f;
	if (g_max_vis_options < g_option_count) {
		add_rect(ImVec2(posx, posy + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_max_vis_options + 1)), g_footer_rect_size, g_footer_rect_color);
		if (g_current_option == 1)
			add_text("u", ImVec2(posx + arrowOffset, posy + centerArrowOffset + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_max_vis_options + 1)), g_footer_text_color, g_render.m_icon_font);
		else if (g_current_option == g_option_count)
			add_text("d", ImVec2(posx + arrowOffset, posy + centerArrowOffset + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_max_vis_options + 1)), g_footer_text_color, g_render.m_icon_font);
		else {
			add_text("u", ImVec2(posx + arrowOffset, posy + 3.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_max_vis_options + 1)), g_footer_text_color, g_render.m_icon_font);
			add_text("d", ImVec2(posx + arrowOffset, posy + 12.5f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_max_vis_options + 1)), g_footer_text_color, g_render.m_icon_font);
		}
	}
	else {
		add_rect(ImVec2(posx, posy + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count + 1)), g_footer_rect_size, g_footer_rect_color);
		if (g_current_option == 1)
			add_text("u", ImVec2(posx + arrowOffset, posy + centerArrowOffset + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count + 1)), g_footer_text_color, g_render.m_icon_font);
		else if (g_current_option == g_option_count)
			add_text("d", ImVec2(posx + arrowOffset, posy + centerArrowOffset + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count + 1)), g_footer_text_color, g_render.m_icon_font);
		else {
			add_text("u", ImVec2(posx + arrowOffset, posy + 3.f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count + 1)), g_footer_text_color, g_render.m_icon_font);
			add_text("d", ImVec2(posx + arrowOffset, posy + 12.5f + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y * (g_option_count + 1)), g_footer_text_color, g_render.m_icon_font);
		}
	}
}
void add_menu_infobox() {
	auto icon_offset = 7.f;
	auto text_offset = icon_offset + 24.f;
	auto icon_offset_from_rect = 8.f;
	auto text_offset_from_rect = 10.f;
	if (g_max_vis_options < g_option_count) {
		add_rect(ImVec2(posx, posy + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y + g_footer_rect_size.y * (g_max_vis_options + 1)), g_infobox_rect_size, g_infobox_rect_color);
		add_text("i", ImVec2(posx + icon_offset, posy + icon_offset_from_rect + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y + g_footer_rect_size.y * (g_max_vis_options + 1)), g_infobox_text_color, g_render.m_big_icon_font);
		add_text(g_info_text, ImVec2(posx + text_offset, posy + text_offset_from_rect + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y + g_footer_rect_size.y * (g_max_vis_options + 1)), g_infobox_text_color, g_render.m_infobox_font);
	}
	else {
		add_rect(ImVec2(posx, posy + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y + g_footer_rect_size.y * (g_option_count + 1)), g_infobox_rect_size, g_infobox_rect_color);
		add_text("i", ImVec2(posx + icon_offset, posy + icon_offset_from_rect + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y + g_footer_rect_size.y * (g_option_count + 1)), g_infobox_text_color, g_render.m_big_icon_font);
		add_text(g_info_text, ImVec2(posx + text_offset, posy + text_offset_from_rect + g_header_rect_size.y + g_submenu_rect_size.y + g_option_rect_size.y + g_footer_rect_size.y * (g_option_count + 1)), g_infobox_text_color, g_render.m_infobox_font);
	}
}
void change_width() {
	g_header_rect_size.x = width;
	g_option_rect_size.x = width;
	g_submenu_rect_size.x = width;
	g_footer_rect_size.x = width;
	g_infobox_rect_size.x = width;
}

inline std::vector<std::string> g_credits = {
	"Vali",
	"Patchbyte",
	"Poccaking",
};
int g_credits_pos = 0;
bool g_to_enable = true;
int g_change_alpha = 0;
int g_selected_player = 0;
void menu_demo() {
	add_text_option("Option");
	add_toggle_option(&g_to_enable, "Bool");
	add_editor_option(g_change_alpha, 0, 1000, 1, "Int");
	add_vector_option(g_credits, g_credits_pos, "String Vector");
}
void menu_network_selected_player() {
	add_text_option("Placeholder", "Just a placeholder.");
}
void menu_network_players() {
	for (auto i = 0; i < 32; i++)
		if (add_submenu_option(menu_network_selected_player, std::to_string((int)i).c_str(), std::to_string((int)i).c_str(), "Modify Placeholder"))
			g_selected_player = i;
}
void menu_network() {
	add_submenu_option(menu_network_players, "Players", "Players", "View and modify other players.");
}
void menu_settings() {
	add_editor_option(width, 350.f, 450.f, 10.f, "Width", "Changes the menu width. | Does not work"); //Right text is dead when changing width, does work
	add_editor_option(posx, 0.f, ImGui::GetIO().DisplaySize.x, 10.f, "X Position", "Changes the menu X Position.");
	add_editor_option(posy, 0.f, ImGui::GetIO().DisplaySize.y, 10.f, "Y Position", "Changes the menu Y Position.");
	if (add_text_option("Unload", "Unloads the menu."))
		g_Running = false;
	if (add_text_option("Exit", "Exits the game."))
		exit(0);
}
void menu_main() {
	add_submenu_option(menu_demo, "Demo", "Demo");
	add_submenu_option(menu_network, "Network", "Network");
	add_submenu_option(menu_settings, "Settings", "Settings");
}
void reset_key_states() {
	g_open_key_pressed = false;
	g_enter_key_pressed = false;
	g_back_key_pressed = false;
	g_up_key_pressed = false;
	g_down_key_pressed = false;
	g_left_key_pressed = false;
	g_right_key_pressed = false;
}
void check_for_keys() {
	check_if_pressed(g_open_key_pressed, VK_INSERT, g_open_delay);
	check_if_pressed(g_enter_key_pressed, VK_RETURN, g_enter_delay);
	check_if_pressed(g_back_key_pressed, VK_BACK, g_back_delay);
	check_if_pressed(g_up_key_pressed, VK_UP, g_vertical_delay);
	check_if_pressed(g_down_key_pressed, VK_DOWN, g_vertical_delay);
	check_if_pressed(g_left_key_pressed, VK_LEFT, g_horizontal_delay);
	check_if_pressed(g_right_key_pressed, VK_RIGHT, g_horizontal_delay);
}
void input_handler() {
	change_width();
	reset_key_states();
	check_for_keys();
	if (g_open_key_pressed) {
		if (g_menu_level == 0)
			menulevel_move_menu(menu_main, "Home");
		else if (g_current_menu == menu_main)
			g_menu_level = 0;
		else
			menulevel_back_menu();
	}
	else if (g_back_key_pressed) {
		if (g_menu_level > 0)
			menulevel_back_menu();
	}
	else if (g_up_key_pressed) {
		if (g_current_option > 1)
			g_current_option--;
		else
			g_current_option = g_option_count;
	}
	else if (g_down_key_pressed) {
		if (g_current_option < g_option_count)
			g_current_option++;
		else
			g_current_option = 1;
	}
	g_option_count = 0;
}

long __stdcall render::present_hk(IDXGISwapChain* swapchain, UINT syncinterval, UINT flags) {
	if (g_render.initdx) {
		swapchain->GetDesc(&g_render.m_desc);
		swapchain->GetDevice(__uuidof(ID3D11Device), (void**)&g_render.m_device);
		g_render.m_device->GetImmediateContext(&g_render.m_context);
		ImGui::CreateContext();
		ImGui_ImplDX11_Init(g_render.m_device, g_render.m_context);
		ImGui_ImplWin32_Init(g_render.m_desc.OutputWindow);
		ImGui::StyleColorsClassic();
		g_render.m_font_config.FontDataOwnedByAtlas = false;
		//Arial Font Names: Bold Italic = ArialBI | Bold = ArialBD | Black = ArialBLK | Italic = ArialI | Regular = Arial
		g_render.m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 17.f, &g_render.m_font_config);
		g_render.m_subtitle_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 19.f, &g_render.m_font_config);
		g_render.m_header_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\ArialBD.ttf", 40.f, &g_render.m_font_config);
		g_render.m_infobox_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 15.5f, &g_render.m_font_config);
		g_render.m_icon_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)icon_font_data, sizeof(icon_font_data), 15.f, &g_render.m_font_config);
		g_render.m_big_icon_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)icon_font_data, sizeof(icon_font_data), 20.f, &g_render.m_font_config);
		g_render.m_default_font = ImGui::GetDefaultFont();
		g_render.initdx = false;
	}
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	input_handler();
	if (g_menu_level != 0) {
		add_menu_header("Foxy");
		g_current_menu();
		add_menu_subtitle(g_current_subtitle);
		add_menu_footer();
		if (!std::string(g_info_text).empty())
			add_menu_infobox();
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return g_render.directx_present_hook(swapchain, syncinterval, flags);
}
LRESULT render::wndproc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);
	return CallWindowProcW(g_render.wnd_proc_hk, hWnd, Msg, wParam, lParam);
}
void render::start_render() {
	MH_Initialize();
	wnd_proc_hk = (WNDPROC)SetWindowLongPtr(FindWindowA("grcWindow", nullptr), GWLP_WNDPROC, (LONG_PTR)wndproc);
	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		kiero::bind(8, (void**)&directx_present_hook, present_hk);
}
void render::stop_render() {
	SetWindowLongPtr(FindWindowA("grcWindow", nullptr), GWLP_WNDPROC, (LONG_PTR)wnd_proc_hk);
	kiero::unbind(8, true);
	MH_Uninitialize();
}