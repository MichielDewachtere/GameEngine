//#include "stdafx.h"
#include "SDLRenderer.h"

#include <stdexcept>

#include "SceneManager.h"
#include "SDLTexture2D.h"

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

real::SDLRenderer::SDLRenderer(SDL_Window* window, glm::u8vec4 background)
{
	m_BackgroundColor = background;
	Init(window, glmToSDLColor(background));
}

void real::SDLRenderer::Init(SDL_Window* window, SDL_Color /*background*/)
{
	m_pWindow = window;
	m_pRenderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_pRenderer == nullptr) 
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

void real::SDLRenderer::Render() const
{
	//const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_pRenderer, m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);
	SDL_RenderClear(m_pRenderer);

	SceneManager::GetInstance().Render();

#ifdef USE_IMGUI
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_pWindow);
	ImGui::NewFrame();

	//some logic

	if (m_ShowDemo)
		ImGui::ShowDemoWindow(&m_ShowDemo);
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
#endif // USE_IMGUI

	SDL_RenderPresent(m_pRenderer);
}


void real::SDLRenderer::Destroy()
{
#ifdef USE_IMGUI
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
#endif // USE_IMGUI

	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
}

void real::SDLRenderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	RenderTexture(texture, x, y, static_cast<float>(texture.GetSize().x), static_cast<float>(texture.GetSize().y));
}

void real::SDLRenderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	const auto sdlTexture = dynamic_cast<const SDLTexture2D*>(&texture);

	if (sdlTexture == nullptr)
		return;

	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), sdlTexture->GetSDLTexture(), nullptr, &dst);
}

void real::SDLRenderer::RenderSprite(const Texture2D& texture, glm::ivec4 src, glm::ivec4 dst, float angle, 
									glm::ivec2 center, RenderFlip flip)
{
	const SDL_Rect srcRect = glmToSDLRect(src);
	const SDL_Rect dstRect = glmToSDLRect(dst);

	const SDL_Point rotationCenter = glmToSDLPoint(center);

	SDL_RendererFlip sdlFlip = SDL_FLIP_NONE;
	switch (flip)
	{
	case RenderFlip::vertical: 
		sdlFlip = SDL_FLIP_VERTICAL;
		break;
	case RenderFlip::horizontal:
		sdlFlip = SDL_FLIP_HORIZONTAL;
		break;
	case RenderFlip::none:
		sdlFlip = SDL_FLIP_NONE;
		break;
	}

	SDL_RenderCopyEx(GetSDLRenderer(), dynamic_cast<const SDLTexture2D*>(&texture)->GetSDLTexture(), &srcRect, &dstRect, angle, &rotationCenter, sdlFlip);
}

void real::SDLRenderer::RenderRectangle(const glm::ivec4& rect, bool isFilled, glm::u8vec4 color) const
{
	SDL_Color originalColor;
	SDL_GetRenderDrawColor(m_pRenderer, &originalColor.r, &originalColor.g, &originalColor.b, &originalColor.a);

	const SDL_Rect sdlRect{ rect.x, rect.y, rect.z, rect.w };

	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	if (isFilled)
		SDL_RenderFillRect(GetSDLRenderer(), &sdlRect);
	else
		SDL_RenderDrawRect(GetSDLRenderer(), &sdlRect);
	SDL_SetRenderDrawColor(m_pRenderer, originalColor.r, originalColor.g, originalColor.b, originalColor.a);
}

void real::SDLRenderer::RenderPoint(int x, int y, glm::u8vec4 color) const
{
	SDL_Color originalColor;
	SDL_GetRenderDrawColor(m_pRenderer, &originalColor.r, &originalColor.g, &originalColor.b, &originalColor.a);

	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(GetSDLRenderer(), x, y);
	SDL_SetRenderDrawColor(m_pRenderer, originalColor.r, originalColor.g, originalColor.b, originalColor.a);
}

void real::SDLRenderer::RenderLine(int x1, int y1, int x2, int y2, glm::u8vec4 color) const
{
	SDL_Color originalColor;
	SDL_GetRenderDrawColor(m_pRenderer, &originalColor.r, &originalColor.g, &originalColor.b, &originalColor.a);

	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawLine(GetSDLRenderer(), x1, y1, x2, y2);
	SDL_SetRenderDrawColor(m_pRenderer, originalColor.r, originalColor.g, originalColor.b, originalColor.a);
}

void real::SDLRenderer::RenderShape(const glm::ivec2* points, int count, glm::u8vec4 color) const
{
	SDL_Color originalColor;
	SDL_GetRenderDrawColor(m_pRenderer, &originalColor.r, &originalColor.g, &originalColor.b, &originalColor.a);

	// Transform glm::ivec2 to SDL_Point
	const auto sdlPoints = new SDL_Point[count];
	for (int i = 0; i < count; ++i) 
		sdlPoints[i] = glmToSDLPoint(points[i]);
	
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawLines(m_pRenderer, sdlPoints, count);
	SDL_SetRenderDrawColor(m_pRenderer, originalColor.r, originalColor.g, originalColor.b, originalColor.a);
}

void real::SDLRenderer::RenderShape(const std::vector<glm::ivec2>& points, glm::u8vec4 color) const
{
	RenderShape(&points.front(), static_cast<int>(points.size()), color);
}

SDL_Point real::SDLRenderer::glmToSDLPoint(const glm::ivec2& point)
{
	SDL_Point sdlPoint;
	sdlPoint.x = point.x;
	sdlPoint.y = point.y;
	return sdlPoint;
}

SDL_Rect real::SDLRenderer::glmToSDLRect(const glm::ivec4& rect)
{
	SDL_Rect sdlRect;
	sdlRect.x = rect.x;
	sdlRect.y = rect.y;
	sdlRect.w = rect.z;
	sdlRect.h = rect.w;
	return sdlRect;
}

SDL_Color real::SDLRenderer::glmToSDLColor(const glm::u8vec4& color)
{
	SDL_Color sdlColor;
	sdlColor.r = color.x;
	sdlColor.g = color.y;
	sdlColor.b = color.z;
	sdlColor.a = color.w;
	return sdlColor;

}