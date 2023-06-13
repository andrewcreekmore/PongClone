
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

static void drawNumber(int numValue, float numPosition_x, float numPosition_y, float numSize, unsigned int numColor)
{
	float numHalfSize = numSize * .5f;

	bool numberDrawn = false;

	while (numValue || !numberDrawn)
	{
		numberDrawn = true;
		int numDigit = numValue % 10;
		numValue = numValue / 10;

		switch (numDigit)
		{
			case 0:
			{
				drawRect(numPosition_x - numSize, numPosition_y, numHalfSize, 2.5f * numSize, numColor);
				drawRect(numPosition_x + numSize, numPosition_y, numHalfSize, 2.5f * numSize, numColor);
				drawRect(numPosition_x, numPosition_y + (numSize * 2.f), numHalfSize, numHalfSize, numColor);
				drawRect(numPosition_x, numPosition_y - (numSize * 2.f), numHalfSize, numHalfSize, numColor);
				numPosition_x -= numSize * 4.f;
			} break;

			case 1:
			{
				drawRect(numPosition_x + numSize, numPosition_y, numHalfSize, 2.5f * numSize, numColor);
				numPosition_x -= numSize * 2.f;
			}
			break;

			case 2:
			{
				drawRect(numPosition_x, numPosition_y + numSize * 2.f, 1.5f * numSize, numHalfSize, numColor);
				drawRect(numPosition_x, numPosition_y, 1.5f * numSize, numHalfSize, numColor);
				drawRect(numPosition_x, numPosition_y - numSize * 2.f, 1.5f * numSize, numHalfSize, numColor);
				drawRect(numPosition_x + numSize, numPosition_y + numSize, numHalfSize, numHalfSize, numColor);
				drawRect(numPosition_x - numSize, numPosition_y - numSize, numHalfSize, numHalfSize, numColor);
				numPosition_x -= numSize * 4.f;
			} break;

			case 3:
			{
				drawRect(numPosition_x - numHalfSize, numPosition_y + numSize * 2.f, numSize, numHalfSize, numColor);
				drawRect(numPosition_x - numHalfSize, numPosition_y, numSize, numHalfSize, numColor);
				drawRect(numPosition_x - numHalfSize, numPosition_y - numSize * 2.f, numSize, numHalfSize, numColor);
				drawRect(numPosition_x + numSize, numPosition_y, numHalfSize, 2.5f * numSize, numColor);
				numPosition_x -= numSize * 4.f;
			} break;

			case 4:
			{
				drawRect(numPosition_x + numSize, numPosition_y, numHalfSize, 2.5f * numSize, numColor);
				drawRect(numPosition_x - numSize, numPosition_y + numSize, numHalfSize, 1.5f * numSize, numColor);
				drawRect(numPosition_x, numPosition_y, numHalfSize, numHalfSize, numColor);
				numPosition_x -= numSize * 4.f;
			} break;

			case 5:
			{
				drawRect(numPosition_x, numPosition_y + numSize * 2.f, 1.5f * numSize, numHalfSize, numColor);
				drawRect(numPosition_x, numPosition_y, 1.5f * numSize, numHalfSize, numColor);
				drawRect(numPosition_x, numPosition_y - numSize * 2.f, 1.5f * numSize, numHalfSize, numColor);
				drawRect(numPosition_x - numSize, numPosition_y + numSize, numHalfSize, numHalfSize, numColor);
				drawRect(numPosition_x + numSize, numPosition_y - numSize, numHalfSize, numHalfSize, numColor);
				numPosition_x -= numSize * 4.f;
			} break;

			case 6:
			{
				drawRect(numPosition_x + numHalfSize, numPosition_y + numSize * 2.f, numSize, numHalfSize, numColor);
				drawRect(numPosition_x + numHalfSize, numPosition_y, numSize, numHalfSize, numColor);
				drawRect(numPosition_x + numHalfSize, numPosition_y - numSize * 2.f, numSize, numHalfSize, numColor);
				drawRect(numPosition_x - numSize, numPosition_y, numHalfSize, 2.5f * numSize, numColor);
				drawRect(numPosition_x + numSize, numPosition_y - numSize, numHalfSize, numHalfSize, numColor);
				numPosition_x -= numSize * 4.f;
			} break;

			case 7:
			{
				drawRect(numPosition_x + numSize, numPosition_y, numHalfSize, 2.5f * numSize, numColor);
				drawRect(numPosition_x - numHalfSize, numPosition_y + numSize * 2.f, numSize, numHalfSize, numColor);
				numPosition_x -= numSize * 4.f;
			} break;

			case 8:
			{
				drawRect(numPosition_x - numSize, numPosition_y, numHalfSize, 2.5f * numSize, numColor);
				drawRect(numPosition_x + numSize, numPosition_y, numHalfSize, 2.5f * numSize, numColor);
				drawRect(numPosition_x, numPosition_y + numSize * 2.f, numHalfSize, numHalfSize, numColor);
				drawRect(numPosition_x, numPosition_y - numSize * 2.f, numHalfSize, numHalfSize, numColor);
				drawRect(numPosition_x, numPosition_y, numHalfSize, numHalfSize, numColor);
				numPosition_x -= numSize * 4.f;
			} break;

			case 9:
			{
				drawRect(numPosition_x - numHalfSize, numPosition_y + numSize * 2.f, numSize, numHalfSize, numColor);
				drawRect(numPosition_x - numHalfSize, numPosition_y, numSize, numHalfSize, numColor);
				drawRect(numPosition_x - numHalfSize, numPosition_y - numSize * 2.f, numSize, numHalfSize, numColor);
				drawRect(numPosition_x + numSize, numPosition_y, numHalfSize, 2.5f * numSize, numColor);
				drawRect(numPosition_x - numSize, numPosition_y + numSize, numHalfSize, numHalfSize, numColor);
				numPosition_x -= numSize * 4.f;
			} break;
		}
	}
}