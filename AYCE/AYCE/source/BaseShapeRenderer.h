/*****************************************************************************/
/*!
\file   BaseShapeRenderer.h
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for BaseShapeRenderer features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "precompiled.h"

namespace VK
{

	class BaseShapeRenderer
	{
	public:
		enum {
			DRAW_SOLID = 0,
			DRAW_NORMAL,
			TOTAL_DRAW
		};

		BaseShapeRenderer(): m_fColorAlpha(0), m_nSegments(0) { m_nRenderMode = 0; }
		~BaseShapeRenderer() {}

		virtual void Init() = 0;
		virtual void Render() = 0;
		virtual void Destroy() = 0;


		__inline void SetSegments(int seg) { m_nSegments = seg; }
		__inline int GetSegments() const { return m_nSegments; }

		__inline void SetRenderMode(unsigned char mode) { m_nRenderMode = mode; }
		__inline unsigned char GetRenderMode() const { return m_nRenderMode; }

		__inline void SetColor(float r, float g, float b, float a) { m_vColor = glm::vec3(r, g, b); m_fColorAlpha = a; }
		__inline glm::vec3 GetColor() const { return m_vColor; }

		__inline std::vector<glm::vec3>*  GetVertexList() { return &m_vecVertex; }


	protected:
		glm::vec3			m_vColor;
		float				m_fColorAlpha;

		unsigned char	    m_nRenderMode;
		int					m_nSegments;

		std::vector<glm::vec3> m_vecVertex;
	};
} //namespace VK