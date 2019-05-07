#pragma once
#include<Windows.h>
#include<gdiplus.h>
#include<vector>

namespace element {

	class IDrawable {
	protected:
		Gdiplus::Pen* GetDefaultPen();
	public:
		Gdiplus::Pen* pen = NULL;
		virtual void Draw(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pen) = 0;
		void Draw(Gdiplus::Graphics* pGraphics);
	};

	class Point : public IDrawable {
		virtual void Draw(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pen) override;
	public:
		float x;
		float y;
		float size;
		DWORD id;
		Point(float x, float y, float size, DWORD id);
		Point();
	};

	class Line : public IDrawable {
		virtual void Draw(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pen) override;
	public:
		Point start;
		Point end;
		Line();
	};

	class LineStrip : public IDrawable {
		virtual void Draw(Gdiplus::Graphics* pGraphics, Gdiplus::Pen* pen) override;
	public:
		std::vector<Point> points;
	};
}

struct ELEMENT_TRANSFORM {
	float x=0;
	float y=0;
	float scale=1;
	float rotate=0;
};

class Layer {
public:
	ELEMENT_TRANSFORM transform;
	std::vector<element::IDrawable*> elements;

	//void GetMatrix(Gdiplus::Matrix& matrix);


};


class Model {
public:
	ELEMENT_TRANSFORM transform;
	std::vector<Layer*> layers;

	Model(int layer_count);
	Model();

};

class Controller {
	//std::vector<Layer*> layers;

	Model* model;


	bool isDrag = false;
	int drag_enter_x = 0;
	int drag_enter_y = 0;
	int old_transform_x = 0;
	int old_transform_y = 0;
	int cur_mouse_x = 0;
	int cur_mouse_y = 0;

public:
	Controller(Model* model);

	BOOL OnNetMessage(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL OnViewTransform(UINT message, WPARAM wParam, LPARAM lParam);
};





class View {
	bool external_graphics = false;
protected:
	Gdiplus::Graphics* pGraphics;
public:

	void Init(HDC hdc);

	View(Gdiplus::Graphics* pGraphics);
	View(HDC hdc);
	View();

	void ResetTransform(ELEMENT_TRANSFORM* transform);
	void PaintLayer(Layer* layer);
	void PaintTransformedLayer(Layer* layer, ELEMENT_TRANSFORM* transform);
	void PaintModel(Model* model);

	virtual void DrawBackground(Model * model);
	virtual void DrawForeground(Model * model);
	~View();
};


class AxisView : public View {
	virtual void DrawBackground(Model * model) override;
};




//
//struct TRANSFORM {
//	float zoom = 1;
//	float rotate = 0;
//	float cx = 400;
//	float cy = 300;
//};
//
//
//#include<math.h>
//void GetTransformed(TRANSFORM& transform, float x, float y, float* out_x, float* out_y) {
//	float theta = transform.rotate;
//	float rx = cos(theta)*x - sin(theta)*y;
//	float ry = sin(theta)*x + cos(theta)*y;
//
//	rx *= transform.zoom;
//	ry *= transform.zoom;
//
//	*out_x = transform.cx + x;
//	*out_y = transform.cy - y;
//}