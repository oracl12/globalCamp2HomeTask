#include "../headers/renderer.h"

void Renderer::SetPixel(int x, int y, const RGBColor &color)
{
	BitmapBuffer &buffer = getInstance().buffer;

	// clipping
	if (x < 0 || x > buffer.width || y < 0 || y > buffer.height)
		return;

	// convert (u8, u8, u8) rgb color to u32 representation
	uint32_t raw_color = (color.red << 16) | (color.green << 8) | (color.blue << 0);

	uint8_t *row = (uint8_t *)buffer.memory + x * bytes_per_pixel + y * buffer.pitch;
	uint32_t *pixel = (uint32_t *)row;
	*pixel = raw_color;
}

void Renderer::FillRectangle(const Rect &rect, const RGBColor &color)
{
	BitmapBuffer &buffer = getInstance().buffer;

	int minX = rect.x;
	int minY = rect.y;
	int maxX = rect.x + rect.width;
	int maxY = rect.y + rect.height;

	// clipping
	if (minX < 0)
		minX = 0;
	if (minY < 0)
		minY = 0;
	if (maxX > buffer.width)
		maxX = buffer.width;
	if (maxY > buffer.height)
		maxY = buffer.height;

	// convert (u8, u8, u8) rgb color to u32 representation
	uint32_t raw_color = (color.red << 16) | (color.green << 8) | (color.blue << 0);

	uint8_t *row = (uint8_t *)buffer.memory + minX * bytes_per_pixel + minY * buffer.pitch;
	for (int y = minY; y < maxY; y++)
	{
		uint32_t *pixel = (uint32_t *)row;
		for (int x = minX; x < maxX; x++)
		{
			*pixel++ = raw_color;
		}
		row += buffer.pitch;
	}
}

void Renderer::FillRectangleWithBorder(const Rect &rect, const RGBColor &color, const RGBColor &borderColor)
{
	BitmapBuffer &buffer = getInstance().buffer;

	int minX = rect.x;
	int minY = rect.y;
	int maxX = rect.x + rect.width;
	int maxY = rect.y + rect.height;

	// clipping
	if (minX < 0)
		minX = 0;
	if (minY < 0)
		minY = 0;
	if (maxX > buffer.width)
		maxX = buffer.width;
	if (maxY > buffer.height)
		maxY = buffer.height;

	// Convert (u8, u8, u8) RGB colors to u32 representations
	uint32_t fillRawColor = (color.red << 16) | (color.green << 8) | (color.blue << 0);
	uint32_t borderRawColor = (borderColor.red << 16) | (borderColor.green << 8) | (borderColor.blue << 0);

	uint8_t *row = (uint8_t *)buffer.memory + minX * bytes_per_pixel + minY * buffer.pitch;
	for (int y = minY; y < maxY; y++)
	{
		uint32_t *pixel = (uint32_t *)row;
		for (int x = minX; x < maxX; x++)
		{
			// Check if the current pixel is within 2 pixels of the border
			if (x - minX < 2 || maxX - x <= 2 || y - minY < 2 || maxY - y <= 2)
			{
				*pixel++ = borderRawColor; // Set border color
			}
			else
			{
				*pixel++ = fillRawColor; // Set fill color
			}
		}
		row += buffer.pitch;
	}
}

void Renderer::DrawRectangle(const Rect &rect, const RGBColor &color)
{
	BitmapBuffer &buffer = getInstance().buffer;

	int minX = rect.x;
	int minY = rect.y;
	int maxX = rect.x + rect.width;
	int maxY = rect.y + rect.height;

	// clipping
	if (minX < 0)
		minX = 0;
	if (minY < 0)
		minY = 0;
	if (maxX > buffer.width)
		maxX = buffer.width;
	if (maxY > buffer.height)
		maxY = buffer.height;

	for (int x = minX; x <= maxX; x++)
	{
		SetPixel(x, minY, color);
		SetPixel(x, maxY, color);
	}

	for (int y = minY; y <= maxY; y++)
	{
		SetPixel(minX, y, color);
		SetPixel(maxX, y, color);
	}
}

void Renderer::allocBuffer()
{
	BitmapBuffer &buffer = getInstance().buffer;

	buffer.width = width;
	buffer.height = height;

	buffer.info.bmiHeader.biSize = sizeof(buffer.info.bmiHeader);
	buffer.info.bmiHeader.biWidth = buffer.width;
	buffer.info.bmiHeader.biHeight = -(buffer.height);
	buffer.info.bmiHeader.biPlanes = 1;
	buffer.info.bmiHeader.biBitCount = 32;
	buffer.info.bmiHeader.biCompression = BI_RGB;

	int bufferMemorySize = buffer.width * buffer.height * bytes_per_pixel;
	buffer.memory = VirtualAlloc(0, bufferMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	buffer.pitch = buffer.width * bytes_per_pixel;
}

void Renderer::copyBufferToWindow(HDC deviceContext, bool yourStep)
{
	BitmapBuffer &buffer = getInstance().buffer;
	// TODO fix this blinking
	SetBkMode( deviceContext, TRANSPARENT );
	TextOut(deviceContext, 620, 660, TEXT("READY"),strlen("READY"));
	yourStep ? TextOut(deviceContext, 7, 7, TEXT("Your Step"),strlen("Your Step")) : TextOut(deviceContext, 7, 7, TEXT("Enemys Step"),strlen("Enemys Step"));
	StretchDIBits(
		deviceContext,
		0, 0, width, height,
		0, 0, buffer.width, buffer.height,
		buffer.memory,
		&(buffer.info),
		DIB_RGB_COLORS,
		SRCCOPY);
}

void Renderer::clear()
{
	BitmapBuffer &buffer = getInstance().buffer;

	FillRectangle({0, 0, buffer.width, buffer.height}, getInstance().clearColor);
}