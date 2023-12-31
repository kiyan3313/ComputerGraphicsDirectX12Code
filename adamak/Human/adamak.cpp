#include "d3dUtility.h"

IDirect3DDevice9* Device = 0; 

const int Width  = 1000;
const int Height = 850;
// speed of walking
static float speed = 0.005f;
// human settings
ID3DXMesh* Objects[14];
D3DMATERIAL9 Materials[14];
D3DXMATRIX Worlds[14];

// limits of plane with x, y, z
static float xx = 20.0f;
static float yy = -7.4f;
static float zz = 20.0f;
// degree of hands and legs
static float walk_degree_forward = 0.0f; // y

// light's settings 
static bool lightEnable = true;
// positions of walking movements
static float walk_position_sides = 0.0f; // x
static float walk_position_Yz = 0.0f; // y
static float walk_position_forward = 0.0f; // z

// vertex buffer
IDirect3DVertexBuffer9* x = 0;

// last move for w and s
static std::string last_move = "";
static boolean hands_positivity = true;

// font settings
ID3DXMesh* font_mesh = 0;
D3DXMATRIX result;
// Vertex structure for planes
struct Vertex {
	Vertex() {}

	Vertex(float x, float y, float z,
		float Qx, float Qy, float Qz,
		float u, float v
	) {
		_x = x, _y = y, _z = z, _Qx = Qx, _Qy = Qy, _Qz = Qz, _u = u, _v = v;
	}

	float _x, _y, _z;
	float _Qx, _Qy, _Qz;
	float _u, _v;

	static const DWORD FVF;
};

const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

// function to find the position of objects after rotating in given axes
D3DXMATRIX PositionFinder(float length, float lengthBefore, float elementPosition[], float degreeOrigin,
	float degreeFinal, std::string directionOrigin, std:: string directionFinal) {

	D3DXMATRIX tra2Origin(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	D3DXMATRIX rotInOrigin;
	if (directionOrigin == "x")
		rotInOrigin = D3DXMATRIX(1, 0, 0, 0,
			0, cosf(degreeOrigin), sinf(degreeOrigin), 0,
			0, -sinf(degreeOrigin), cosf(degreeOrigin), 0,
			0, 0, 0, 1);

	else if (directionOrigin == "y")
		rotInOrigin = D3DXMATRIX(cosf(degreeOrigin), 0, -sinf(degreeOrigin), 0,
			0, 1, 0, 0,
			sinf(degreeOrigin), 0, cosf(degreeOrigin), 0,
			0, 0, 0, 1);

	else
		rotInOrigin = D3DXMATRIX(cosf(degreeOrigin), sinf(degreeOrigin), 0, 0,
			-sinf(degreeOrigin), cosf(degreeOrigin), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

	D3DXMATRIX tra2Length(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, -lengthBefore - length / 2, 0, 1);

	D3DXMATRIX rotFromLength;
	if (directionFinal == "x")
		rotFromLength = D3DXMATRIX(1, 0, 0, 0,
			0, cosf(degreeFinal), sinf(degreeFinal), 0,
			0, -sinf(degreeFinal), cosf(degreeFinal), 0,
			0, 0, 0, 1);

	else if (directionFinal == "y")
		rotFromLength = D3DXMATRIX(cosf(degreeFinal), 0, -sinf(degreeFinal), 0,
			0, 1, 0, 0,
			sinf(degreeFinal), 0, cosf(degreeFinal), 0,
			0, 0, 0, 1);

	else
		rotFromLength = D3DXMATRIX(cosf(degreeFinal), sinf(degreeFinal), 0, 0,
			-sinf(degreeFinal), cosf(degreeFinal), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

	D3DXMATRIX tra2Final(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		elementPosition[0], elementPosition[1] + lengthBefore + length / 2, elementPosition[2], 1);

	D3DXMATRIX result = tra2Origin * rotInOrigin * tra2Length * rotFromLength * tra2Final;
	return result;
}

D3DXMATRIX translate(D3DXMATRIX base, float x, float y, float z) {
	D3DXMATRIX translateMatrix(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1);

	D3DXMATRIX result = base * translateMatrix;
	return result;
}

void CreateObjects() {

	D3DXCreateTeapot(Device, &Objects[0], 0);
	D3DXCreateSphere(Device, 1.0f, 20, 20, &Objects[1], 0);
	D3DXCreateTorus(Device, 0.6f, 1.0f, 20, 10, &Objects[2], 0);
	D3DXCreateBox(Device, 0.2F, 2.0F, 0.3F, &Objects[3], 0);
	D3DXCreateBox(Device, 0.2F, 2.0F, 0.3F, &Objects[4], 0);
	D3DXCreateSphere(Device, 0.1f, 20, 20, &Objects[5], 0);
	D3DXCreateSphere(Device, 0.1f, 20, 20, &Objects[6], 0);
	D3DXCreateBox(Device, 0.4F, 2.5F, 0.5F, &Objects[7], 0);
	D3DXCreateBox(Device, 0.4F, 2.5F, 0.5F, &Objects[8], 0);
	D3DXCreateBox(Device, 0.6F, 0.111F, 0.1F, &Objects[9], 0);
	D3DXCreateCylinder(Device, 0.02f, 0.1f, 0.8f, 20, 20, &Objects[10], 0);
	D3DXCreateBox(Device, 4000.9f, 0.1, 5000, &Objects[11], 0);
	D3DXCreateBox(Device, 4000.9f, 0.1, 5000, &Objects[12], 0);
	D3DXCreateSphere(Device, 30.0f, 3.0, 20, &Objects[13], 0);


	D3DXMatrixTranslation(&Worlds[0], 0.0f, -0.6f, 0.0f);
	D3DXMatrixTranslation(&Worlds[1], 0.0f, -1.6f, 0.0f);
	D3DXMatrixTranslation(&Worlds[2], 0.0f, -3.6f, 0.0f);
	D3DXMatrixTranslation(&Worlds[3], 1.4f, -4.6f, -0.2f);
	D3DXMatrixTranslation(&Worlds[4], -1.4f, -4.6f, -0.2f);
	D3DXMatrixTranslation(&Worlds[5], 0.4f, -1.6f, -1.0f);
	D3DXMatrixTranslation(&Worlds[6], -0.4f, -1.6f, -1.0f);
	D3DXMatrixTranslation(&Worlds[7], 0.8f, -5.6f, -0.15f);
	D3DXMatrixTranslation(&Worlds[8], -0.8f, -5.6f, -0.15f);
	D3DXMatrixTranslation(&Worlds[9], 0.02f, -2.1f, -0.9f);
	D3DXMatrixTranslation(&Worlds[10], 0.02f, -1.8f, -1.3f);
	D3DXMatrixTranslation(&Worlds[11], 0.0f,300.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[12], 0.0f, -7.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[13], 0.0f, 30.0f, -10.0f);


	Materials[0] = d3d::GREEN_MTRL;
	Materials[1] = d3d::YELLOW_MTRL;
	Materials[2] = d3d::BLUE_MTRL;
	Materials[3] = d3d::YELLOW_MTRL;
	Materials[4] = d3d::YELLOW_MTRL;
	Materials[5] = d3d::RED_MTRL;
	Materials[6] = d3d::RED_MTRL;
	Materials[7] = d3d::RED_MTRL;
	Materials[8] = d3d::RED_MTRL;
	Materials[9] = d3d::BLUE_MTRL;
	Materials[10] = d3d::BLUE_MTRL;
	Materials[11] = d3d::BLUE_MTRL;
	Materials[12] = d3d::GREEN_MTRL;
	Materials[13] = d3d::YELLOW_MTRL;
	
}

// function to create floor and wall
void CreateHome() {
	Device->CreateVertexBuffer(
		30 * sizeof(Vertex),
		0,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&x,
		0);

	Vertex* v = 0;
	x->Lock(0, 0, (void**)&v, 0);
	
	// back wall vertex
	v[0] = Vertex(-xx,yy,zz,0.0f,0.0f,-0.5f,0.0f,0.5f);
	v[1] = Vertex(-xx,-yy+ 7,zz, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f);
	v[2] = Vertex(xx,-yy+ 7,zz, 0.0f, 0.0f, -0.5f, 0.5f, 0.0f);

	v[3] = Vertex(-xx,yy,zz, 0.0f, 0.0f, -0.5f, 0.0f, 0.5f);
	v[4] = Vertex(xx,-yy + 7,zz, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f);
	v[5] = Vertex(xx, yy, zz, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f);

	// left wall vertex
	v[6] = Vertex(-xx, yy, -zz, 0.0f, 0.0f, -0.5f, 0.0f, 0.5f);
	v[7] = Vertex(-xx, -yy + 7, -zz, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f);
	v[8] = Vertex(-xx,- yy + 7,zz, 0.0f, 0.0f, -0.5f, 0.5f, 0.0f);

	v[9] = Vertex(-xx, yy, -zz, 0.0f, 0.0f, -0.5f, 0.0f, 0.5f);
	v[10] = Vertex(-xx, -yy + 7, zz, 0.0f, 0.0f, -0.5f, 0.5f, 0.0f);
	v[11] = Vertex(-xx, yy, zz, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f);

	// right wall vertex
	v[12] = Vertex(xx, yy,-zz, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f);
	v[13] = Vertex(xx, -yy + 7, -zz, 0.5f, 0.5f, -0.5f, 0.5f, 0.0f);
	v[14] = Vertex(xx, -yy + 7, zz, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f);

	v[15] = Vertex(xx, yy, -zz, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f);
	v[16] = Vertex(xx, -yy + 7, zz, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f);
	v[17] = Vertex(xx, yy, zz, 0.5f, 0.5f, -0.5f, 0.0f, -0.5f);

	// roof
	v[18] = Vertex(-xx, -yy + 7, -zz, 0.5f, 0.0f, 0.5f, 0.0f, 0.5f);
	v[19] = Vertex(-xx, -yy + 7, zz, 0.5f, 0.0f, 0.5f, 0.0f, 0.0f);
	v[20] = Vertex(xx, -yy + 7, zz, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f);

	v[21] = Vertex(-xx, -yy + 7, -zz, 0.5f, 0.0f, 0.5f, 0.0f, 0.5f);
	v[22] = Vertex(xx, -yy + 7, zz, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f);
	v[23] = Vertex(xx, -yy + 7, -zz, 0.5f, 0.0f, -0.5f, 0.5f, 0.5f);
	//miror

	v[24] = Vertex(-3.0f, yy, zz - 0.01, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
		v[25] = Vertex(-3.0f, 1, zz - 0.01, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
		v[26] = Vertex(3.0f, 1, zz - 0.01, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),

		v[27] = Vertex(-3.0f, yy, zz - 0.00, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
		v[28] = Vertex(3.0f, 1, zz - 0.01, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
		v[29] = Vertex(3.0f, yy, zz - 0.01, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
		x->Unlock();
}

// function for setting light
void Light() {
	// Set up a directional light.
	D3DXVECTOR3 dir(2.0f, 2.0f, 2.0f);
	D3DXCOLOR c;

	// change light color
	c = d3d::WHITE;

	D3DLIGHT9 dirLight = d3d::InitDirectionalLight(&dir, &c);

	// Set and Enable the light.
	Device->SetLight(0, &dirLight);
	Device->LightEnable(0, lightEnable);

	// Set lighting related render states.
	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, false);

	// Set the projection matrix.
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.25f, // 45 - degree
		(float)Width / (float)Height,
		1.0f,
		1000.0f);

	Device->SetTransform(D3DTS_PROJECTION, &proj);
}
	void Cleanup(){
		d3d::Release<IDirect3DVertexBuffer9*>(x);
	for(int i = 0; i < 14; i++)
		d3d::Release<ID3DXMesh*>(Objects[i]);
}
	// function to reset positions of the body to 90 degrees
	void resetBodyWall() {
		Worlds[7] = PositionFinder(3.0f, -1, new float[] {-2.85f - walk_position_sides, -2.5f + walk_position_Yz,
			0.0f - walk_position_forward}, 3.14 / 2, 0, "X", "x");

		Worlds[8] = PositionFinder(3.0f, -1, new float[] {+2.85f - walk_position_sides, -2.5f + walk_position_Yz,
			0.0f - walk_position_forward}, 3.14 / 2, 0, "x", "x");

		Worlds[3] = PositionFinder(3.0f, 0, new float[] {-1.2f - walk_position_sides, -5.8f + walk_position_Yz,
			0.0f - walk_position_forward}, 3.14 / 2, 0, "x", "x");

		Worlds[4] = PositionFinder(3.0f, 0, new float[] {1.2f - walk_position_sides, -5.8f + walk_position_Yz,
			0.0f - walk_position_forward}, 3.14 / 2, 0, "x", "x");
	}

	
	// function to walk forward and backward in Z
	void walkForBack(float degreeToRotate,std:: string direction) { // z
		// prevent from overflowing the plane
		if ((direction == "w" && walk_position_forward > zz + 10) ||
			(direction == "s" && walk_position_forward < -zz + 10)) {
			resetBodyWall();
		return;
		}

		if (direction == "w") walk_position_forward += speed;
		if (direction == "s") walk_position_forward -= speed;

		// legs new position
		Worlds[7] = PositionFinder(3.0f, -1, new float[] {-0.8f - walk_position_sides, -5.65f + walk_position_Yz,
			-0.0f - walk_position_forward}, 1.0 / 64.9, degreeToRotate, "x", "x");
		Worlds[8] = PositionFinder(3.0f, -1, new float[] {0.8f - walk_position_sides, -5.65f + walk_position_Yz,
			-0.0f - walk_position_forward}, 1.0 /64.9, -degreeToRotate, "x",
			"x");
		Device->SetTransform(D3DTS_WORLD, &Worlds[7]);
		Objects[7]->DrawSubset(0);
		Device->SetTransform(D3DTS_WORLD, &Worlds[8]);
		Objects[8]->DrawSubset(0);

		// hands new position
		Worlds[3] = PositionFinder(3.0f, 0, new float[] {-1.3f - walk_position_sides, -4.5f + walk_position_Yz,
			-0.3f - walk_position_forward}, 1.0 / 4.9, degreeToRotate, "x", "x");
		Worlds[4] = PositionFinder(3.0f, 0, new float[] {+1.3f - walk_position_sides, -4.5f + walk_position_Yz,
			-0.3f - walk_position_forward}, 1.0 / 4.9, -degreeToRotate, "x",
			"x");
		Device->SetTransform(D3DTS_WORLD, &Worlds[3]);
		Objects[3]->DrawSubset(0);
		Device->SetTransform(D3DTS_WORLD, &Worlds[4]);
		Objects[4]->DrawSubset(0);

		// translate all objects a "speed"
		if (direction == "w")
			for (int i = 0; i < 11; i++)
				Worlds[i] = translate(Worlds[i], 0.0f, 0.0f, -speed);

		else
			for (int i = 0; i < 11; i++)
				Worlds[i] = translate(Worlds[i], 0.0f, 0.0f, speed);
	}
	// function to walk forward and backward in X
	void walkLeftRight(std::string direction) {
		// prevent from overflowing the plane
		if ((direction == "a" && walk_position_sides >xx - 2) ||
			(direction == "d" && walk_position_sides <-xx + 2))
			return;

		if (direction == "a") walk_position_sides += speed;
		if (direction == "d") walk_position_sides -= speed;

		// translate all objects a "speed"
		if (direction == "a")
			for (int i = 0; i < 11; i++)
				Worlds[i] = translate(Worlds[i], -speed, 0.0f, 0.0f);

		else
			for (int i = 0; i < 11; i++)
				Worlds[i] = translate(Worlds[i], speed, 0.0f, 0.0f);
	}
	


	//mirror
		D3DMATERIAL9 mirror_material = d3d::WHITE_MTRL;
	IDirect3DTexture9* mirror_texture = 0;

	void RenderScene() {
		D3DXMATRIX I;
		D3DXMatrixIdentity(&I);
		Device->SetTransform(D3DTS_WORLD, &I);

		Device->SetStreamSource(0, x, 0, sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);

		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		// draw the floor
		Device->SetMaterial(&d3d::YELLOW_MTRL);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		// draw the wall
		Device->SetMaterial(&d3d::RED_MTRL);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 12, 2);
		// draw the wall
		Device->SetMaterial(&d3d::RED_MTRL);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 6, 2);
		// draw the roof
		Device->SetMaterial(&d3d::RED_MTRL);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
		//miror
		Device->SetMaterial(&mirror_material);
		Device->SetTexture(0, mirror_texture);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 24, 2);
	}

	// function to render the shadow
	void RenderShadow() {
		Device->SetRenderState(D3DRS_STENCILENABLE, true);
		Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		Device->SetRenderState(D3DRS_STENCILREF, 0x0);
		Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
		Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
		Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR); // increment to 1

		// position shadow
		D3DXVECTOR4 lightDirection(2.0f, 2.0f, 2.0f, 0.0f);
		D3DXPLANE groundPlane(0.0f, 0.1f,0.0f, 0.0f);

		D3DXMATRIX W, S;
		D3DXMATRIX tra = {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				yy, yy, yy, 1.0f
		};

		D3DXMatrixShadow(
			&S,
			&lightDirection,
			&groundPlane);

		S *= tra;

		int object_index;

		// alpha blend the shadow
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		D3DMATERIAL9 mtrl = d3d::InitMtrl(d3d::BLACK, d3d::BLACK, d3d::BLACK, d3d::BLACK, 0.0f);
		mtrl.Diffuse.a = 1.5f; // 50% transparency.
		
		//Disable depth buffer so that z-fighting doesn't occur when we render the shadow on top of the floor.
		Device->SetRenderState(D3DRS_ZENABLE, false);

		for (int i = 0; i < 10; i++) {
			W = Worlds[i] * S;
			Device->SetTransform(D3DTS_WORLD, &W);
			Device->SetMaterial(&mtrl);
			Objects[i]->DrawSubset(0);
		}

		Device->SetRenderState(D3DRS_ZENABLE, true);
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		Device->SetRenderState(D3DRS_STENCILENABLE, false);
	}


	// function to create the name and font
	void createName() {
		// Get a handle to a device context.0
		HDC hdc = CreateCompatibleDC(0);
		HFONT hFont;
		HFONT hFontOld;

		// Describe the font we want.
		LOGFONT x;
		ZeroMemory(&x, sizeof(LOGFONT));

		x.lfHeight = 1;    // in logical units
		x.lfWidth = 1;    // in logical units
		x.lfEscapement = 0;
		x.lfOrientation = 600000;
		x.lfWeight = 900000;   // boldness, range 0(light) - 1000(bold)
		x.lfItalic = false;
		x.lfUnderline = false;
		x.lfStrikeOut = false;
		x.lfCharSet = DEFAULT_CHARSET;
		x.lfOutPrecision = 0;
		x.lfClipPrecision = 0;
		x.lfQuality = 0;
		x.lfPitchAndFamily = 0;
		// Create the font and select it with the device context.
		hFont = CreateFontIndirect(&x);
		hFontOld = (HFONT)SelectObject(hdc, hFont);

		// Create the text mesh based on the selected font in the HDC.
		D3DXCreateTextA(Device, hdc, "Kiyan Amiri", 0.001f, 0.4f, &font_mesh, 0, 0);

		// Restore the old font and free the acquired HDC.
		SelectObject(hdc, hFontOld);
		DeleteObject(hFont);
		DeleteDC(hdc);

		D3DXMatrixTranslation(&result, -2.3f, 3.0f, zz+5);
	}

	void RenderMirror() {
		Device->SetRenderState(D3DRS_STENCILENABLE, true);
		Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		Device->SetRenderState(D3DRS_STENCILREF, 0x1);
		Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
		Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
		Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

		// disable writes to the depth and back buffers
		Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// draw the mirror to the stencil buffer
		Device->SetStreamSource(0, x, 0, sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);
		Device->SetMaterial(&mirror_material);
		Device->SetTexture(0, mirror_texture);

		D3DXMATRIX I;
		D3DXMatrixIdentity(&I);
		Device->SetTransform(D3DTS_WORLD, &I);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 24, 2);

		// re-enable depth writes
		Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

		// only draw reflected to the pixels where the mirror was drawn to.
		Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

		// position reflection
		D3DXMATRIX W;
		D3DXMATRIX refl = {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, -1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
		};

		D3DXMATRIX tra = {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f,zz , 1.0f
		};

		refl *= tra;

		Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);


		// find the exact reflection in mirror
		W = Worlds[0] * refl;
		Device->SetTransform(D3DTS_WORLD, &W);
		Device->SetMaterial(&Materials[0]);
		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		Objects[0]->DrawSubset(0);

		W = Worlds[1] * refl;
		Device->SetTransform(D3DTS_WORLD, &W);
		Device->SetMaterial(&Materials[1]);
		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		Objects[1]->DrawSubset(0);

		W = Worlds[7] * refl;
		Device->SetTransform(D3DTS_WORLD, &W);
		Device->SetMaterial(&Materials[7]);
		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		Objects[7]->DrawSubset(0);

		W = Worlds[8] * refl;
		Device->SetTransform(D3DTS_WORLD, &W);
		Device->SetMaterial(&Materials[8]);
		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		Objects[8]->DrawSubset(0);

		W = Worlds[4] * refl;
		Device->SetTransform(D3DTS_WORLD, &W);
		Device->SetMaterial(&Materials[4]);
		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		Objects[4]->DrawSubset(0);

		W = Worlds[2] * refl;
		Device->SetTransform(D3DTS_WORLD, &W);
		Device->SetMaterial(&Materials[2]);
		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		Objects[2]->DrawSubset(0);

		W = Worlds[3] * refl;
		Device->SetTransform(D3DTS_WORLD, &W);
		Device->SetMaterial(&Materials[3]);
		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		Objects[3]->DrawSubset(0);

		// Restore render states.
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		Device->SetRenderState(D3DRS_STENCILENABLE, false);
		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	
bool Display(float timeDelta)
{
	// variables for keeping the position of the camera
	static float pos_x = 0.0f;
	static float pos_y = 0.0f;
	static float pos_z = -20.0f;
	// variables for keeping the position of the target
	static float target_x = 0.0f;
	static float target_y = -4.0f;
	static float target_z = 0.0f;
	if (Device)
	{
		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f) // left
			target_x -= 3.5f * timeDelta;

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f) // right
			target_x += 3.5f * timeDelta;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f) // up
			target_y += 5.0f * timeDelta;

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f) // down
			target_y -= 5.0f * timeDelta;

		// change the camera position
		if (GetKeyState(VK_SHIFT) & 0x8000f) // x
			if (::GetAsyncKeyState(0x58) & 0x8000f)
				pos_x += 15.0f * timeDelta;

		if (GetKeyState(VK_SHIFT) != 1) // x
			if (::GetAsyncKeyState(0x58) & 0x8000f)
				pos_x -= 15.0f * timeDelta;

		if (GetKeyState(VK_SHIFT) & 0x8000f) // y
			if (::GetAsyncKeyState(0x59) & 0x8000f)
				pos_y += 15.0f * timeDelta;

		if (GetKeyState(VK_SHIFT) != 1) // y
			if (::GetAsyncKeyState(0x59) & 0x8000f)
				pos_y -= 15.0f * timeDelta;

		if (GetKeyState(VK_SHIFT) & 0x8000f) // z
			if (::GetAsyncKeyState(0x5a) & 0x8000f)
				pos_z += 15.0f * timeDelta;

		if (GetKeyState(VK_SHIFT) != 1) // z
			if (::GetAsyncKeyState(0x5a) & 0x8000f)
				pos_z -= 15.0f * timeDelta;
		// move forward
		if (::GetAsyncKeyState(0x57) & 0x8000f) { // w

			// walk back if last move is different from w
			if (last_move == "s") {
				if (walk_degree_forward > 0) {
					for (int x = 0; x < 8000000; x++) {
						if (x % 100 == 0) {
							walk_degree_forward -= speed / 2.0f;
							walkForBack(-walk_degree_forward, "w");
						}

						if (walk_degree_forward > 0)
							break;
					}

				}
				else {
					for (int x = 0; x < 1000000; x++) {
						if (x % 100 == 0) {
							walk_degree_forward += speed / 2.0f;
							walkForBack(-walk_degree_forward, "w");
						}

						if (walk_degree_forward < 0)
							break;
					}
				}

				Sleep(100);
				last_move = "w";

			}
			else {
				last_move = "w";

				if (hands_positivity)
					walk_degree_forward += speed;

				else
					walk_degree_forward -= speed;

				walkForBack(walk_degree_forward, "w");

				if (walk_degree_forward >= 3.14f / 4)
					hands_positivity = false;

				else if (walk_degree_forward <= -3.14f / 4)
					hands_positivity = true;
			}
		}

		// move backward
		if (::GetAsyncKeyState(0x53) & 0x8000f) { // s

			// walk back if last move is different from s
			if (last_move == "w") {
				if (walk_degree_forward > 0) {
					for (int x = 0; x < 1000000; x++) {
						if (x % 100 == 0) {
							walk_degree_forward -= speed / 2.0f;
							walkForBack(walk_degree_forward, "s");
						}

						if (walk_degree_forward > 0)
							break;
					}
				}
				else {
					for (int x = 0; x < 1000000; x++) {
						if (x % 100 == 0) {
							walk_degree_forward += speed / 2.0f;
							walkForBack(walk_degree_forward, "s");
						}

						if (walk_degree_forward < 0)
							break;
					}
				}

				Sleep(100);
				last_move = "s";

			}
			else {
				last_move = "s";

				if (hands_positivity)
					walk_degree_forward += speed / 2.0f;

				else
					walk_degree_forward -= speed / 2.0f;

				walkForBack(-walk_degree_forward, "s");

				if (walk_degree_forward >= 3.14f / 6)
					hands_positivity = false;

				else if (walk_degree_forward <= -3.14f / 6)
					hands_positivity = true;
			}
		}

		//move left
		if (::GetAsyncKeyState(0x41) & 0x8000f) { // a
			walkLeftRight("a");
		}

		//move right
		if (::GetAsyncKeyState(0x44) & 0x8000f) { // d
			walkLeftRight("d");

		}
		// setting camera
		D3DXVECTOR3 position(pos_x, pos_y, pos_z);
		D3DXVECTOR3 target(target_x, target_y, target_z);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);

		Device->SetTransform(D3DTS_VIEW, &V);
		// Draw the scene:
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xff000000, 1.0f, 0L);
		Device->BeginScene();
		RenderScene();


		for (int i = 0; i < 14; i++)
		{
			Device->SetMaterial(&Materials[i]);
			Device->SetTransform(D3DTS_WORLD, &Worlds[i]);
			Objects[i]->DrawSubset(0);
		}
		
		RenderShadow();

		RenderMirror();


		// draw font and name, set material and world matrix
		Device->SetTransform(D3DTS_WORLD, &result);
		Device->SetMaterial(&d3d::BLUE_MTRL);
		font_mesh->DrawSubset(0);

		
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

bool Setup() {

	CreateObjects();
	CreateHome();
	createName();
	Light();
	return true;
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
// WinMain
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0,(LPCWSTR) "InitD3D() - FAILED", 0, 0);
		return 0;
	}
	if (!Setup())
	{
		::MessageBox(0, (LPCWSTR)"Setup() - FAILED", 0, 0);
		
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}