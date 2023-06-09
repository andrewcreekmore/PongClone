static float renderScale = 0.01f;


static void clearScreen(unsigned int color)
{
	unsigned int* pixel = (unsigned int*)renderState.memory;
	for (int y = 0; y < renderState.height; y++)
	{
		for (int x = 0; x < renderState.width; x++)
		{ *pixel++ = color; }
	}
}


static void drawRectInPixels(int x0, int y0, int x1, int y1, unsigned int color)
{
	// don't exceed window
	x0 = clamp(0, x0, renderState.width);
	x1 = clamp(0, x1, renderState.width);
	y0 = clamp(0, y0, renderState.height);
	y1 = clamp(0, y1, renderState.height);

	for (int y = y0; y < y1; y++)
	{
		unsigned int* pixel = (unsigned int*)renderState.memory + x0 + y * renderState.width;
		for (int x = x0; x < x1; x++)
		{ *pixel++ = color; }
	}
}


static void drawRect(float x, float y, float halfSize_x, float halfSize_y, unsigned int color)
{
	// multiply position and halfSize by window height to make relative to window, regardless of window size
	x *= renderState.height * renderScale;
	y *= renderState.height * renderScale;
	halfSize_x *= renderState.height * renderScale;
	halfSize_y *= renderState.height * renderScale;

	// offset position to center of screen by adding half of width/height
	x += renderState.width / 2.f;
	y += renderState.height / 2.f;

	// change to pixels
	int x0 = static_cast<int>(x) - static_cast<int>(halfSize_x);
	int x1 = static_cast<int>(x) + static_cast<int>(halfSize_x);
	int y0 = static_cast<int>(y) - static_cast<int>(halfSize_y);
	int y1 = static_cast<int>(y) + static_cast<int>(halfSize_y);

	drawRectInPixels(x0, y0, x1, y1, color);
}