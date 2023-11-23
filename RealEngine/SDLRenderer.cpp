//#include "stdafx.h"
#include "SDLRenderer.h"

#include <stdexcept>

#include "SceneManager.h"
#include "Texture2D.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void real::SDLRenderer::Init(SDL_Window* window, SDL_Color background)
{
	m_window = window;
	m_clearColor = background;

	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

#ifdef USE_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
#endif // USE_IMGUI
}

void real::SDLRenderer::Render()
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

#ifdef USE_IMGUI
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	//some logic

	if (m_ShowDemo)
		ImGui::ShowDemoWindow(&m_ShowDemo);
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
#endif // USE_IMGUI

	SDL_RenderPresent(m_renderer);
}

void real::SDLRenderer::Destroy()
{
#ifdef USE_IMGUI
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
#endif // USE_IMGUI

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void real::SDLRenderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void real::SDLRenderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void real::SDLRenderer::RenderRectangle(const SDL_Rect& rect, bool isFilled, SDL_Color color) const
{
	SDL_Color originalColor;
	SDL_GetRenderDrawColor(m_renderer, &originalColor.r, &originalColor.g, &originalColor.b, &originalColor.a);

	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, 255);
	if (isFilled)
		SDL_RenderFillRect(GetSDLRenderer(), &rect);
	else
		SDL_RenderDrawRect(GetSDLRenderer(), &rect);
	SDL_SetRenderDrawColor(m_renderer, originalColor.r, originalColor.g, originalColor.b, originalColor.a);
}

void real::SDLRenderer::RenderPoint(const int x, const int y, SDL_Color color) const
{
	SDL_Color originalColor;
	SDL_GetRenderDrawColor(m_renderer, &originalColor.r, &originalColor.g, &originalColor.b, &originalColor.a);

	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawPoint(GetSDLRenderer(), x, y);
	SDL_SetRenderDrawColor(m_renderer, originalColor.r, originalColor.g, originalColor.b, originalColor.a);
}

void real::SDLRenderer::RenderLine(const int x1, const int y1, const int x2, const int y2, SDL_Color color) const
{
	SDL_Color originalColor;
	SDL_GetRenderDrawColor(m_renderer, &originalColor.r, &originalColor.g, &originalColor.b, &originalColor.a);

	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawLine(GetSDLRenderer(), x1, y1, x2, y2);
	SDL_SetRenderDrawColor(m_renderer, originalColor.r, originalColor.g, originalColor.b, originalColor.a);
}

void real::SDLRenderer::RenderShape(const SDL_Point* points, int count, SDL_Color color) const
{
	SDL_Color originalColor;
	SDL_GetRenderDrawColor(m_renderer, &originalColor.r, &originalColor.g, &originalColor.b, &originalColor.a);

	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawLines(m_renderer, points, count);
	SDL_SetRenderDrawColor(m_renderer, originalColor.r, originalColor.g, originalColor.b, originalColor.a);
}

void real::SDLRenderer::RenderShape(const std::vector<SDL_Point>& points, SDL_Color color) const
{
	RenderShape(&points.front(), static_cast<int>(points.size()), color);
}

inline SDL_Renderer* real::SDLRenderer::GetSDLRenderer() const { return m_renderer; }
