#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "textureshaderclass.h"
#include "BitmapClass.h"
#include "graphicsclass.h"
#include "GameStateClass.h"
#include "InputClass.h"
#include <cstdlib>
#include <ctime>


GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Direct3D 객체 생성
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Direct3D 객체 초기화
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// m_Camera 객체 생성
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// 카메라 포지션 설정
	m_Camera->SetPosition(0.0f, 0.0f, -6.0f);


	// m_TextureShader 객체 생성
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// m_TextureShader 객체 초기화
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the Texture shader object.", L"Error", MB_OK);
		return false;
	}

	//m_DestBitmapObjects 초기화 이 배열에 들어가면 화면에 렌더링됨
	m_DestBitmapObjects = new BitmapClass*[m_BitmapArrayLength];
	for (int i = 0; i < m_BitmapArrayLength; ++i)
	{
		m_DestBitmapObjects[i] = nullptr;
	}

	//m_MyCellArray 2차원 배열 초기화
	for (int i = 0; i < 10; ++i)
	{

		for (int j = 0; j < 10; ++j)
		{
			m_MyCellArray[i][j] = new BitmapClass;
			m_EnemyCellArray[i][j] = new BitmapClass;

			if (!m_MyCellArray[i][j]->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Cell.png", 80, 80))
			{
				MessageBox(hwnd, L"Could not initialize the bitmap(cell) object.", L"Error", MB_OK);
				return false;
			}

			if (!m_EnemyCellArray[i][j]->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Cell.png", 80, 80))
			{
				MessageBox(hwnd, L"Could not initialize the bitmap(cell) object.", L"Error", MB_OK);
				return false;
			}

			m_MyCellArray[i][j]->SetNextPosX(80 + i * 80);
			m_MyCellArray[i][j]->SetNextPosY(140 + j * 80);
			m_MyCellArray[i][j]->SetTextureUV(0.0f, 0.33333f, 0.0f, 1.0f);

			m_EnemyCellArray[i][j]->SetNextPosX(1040 + i * 80);
			m_EnemyCellArray[i][j]->SetNextPosY(140 + j * 80);
			m_EnemyCellArray[i][j]->SetTextureUV(0.0f, 0.33333f,0.0f ,1.0f);

		}
	}

	//백그라운드 객체 생성
	m_BackGround = new BitmapClass;
	if (!m_BackGround)
	{
		return false;
	}

	//백그라운드 객체 초기화
	if (!m_BackGround->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/BackGround.png", 1920, 1080))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(backGround) object.", L"Error", MB_OK);
		return false;
	}

	//마이맵 객체 생성
	m_MyMap = new BitmapClass;
	if (!m_MyMap)
	{
		return false;
	}

	//마이맵 객체 초기화
	if (!m_MyMap->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Map.png", 800, 800))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(map) object.", L"Error", MB_OK);
		return false;
	}

	//마이맵 위치 설정
	m_MyMap->SetNextPosX(80);
	m_MyMap->SetNextPosY(140);

	//적 맵 객체 생성
	m_EnemyMap = new BitmapClass;
	if (!m_EnemyMap)
	{
		return false;
	}

	//적맵 객체 초기화
	if (!m_EnemyMap->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Map.png", 800, 800))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(map) object.", L"Error", MB_OK);
		return false;
	}
	//적 맵 위치 설정
	m_EnemyMap->SetNextPosX(1040);
	m_EnemyMap->SetNextPosY(140);

	//인트로 객체 생성
	m_Intro = new BitmapClass;
	if (!m_Intro)
	{
		return false;
	}
	//인트로 객체 초기화
	if (!m_Intro->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Intro.png", 1920, 1080))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(map) object.", L"Error", MB_OK);
		return false;
	}

	//메인 신 객체 생성
	m_MainScene = new BitmapClass;
	if (!m_MainScene)
	{
		return false;
	}
	//메인 씬 객체 초기화
	if (!m_MainScene->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/MainScene.png", 1920, 1080))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(map) object.", L"Error", MB_OK);
		return false;
	}

	//게임소개 씬 객체 생성
	m_IntroductionScene = new BitmapClass;
	if (!m_IntroductionScene)
	{
		return false;
	}
	//게임소개 씬 객체 초기화
	if (!m_IntroductionScene->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/IntroductionScene.png", 1920, 1080))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(map) object.", L"Error", MB_OK);
		return false;
	}

	//게임방법 씬 객체 생성
	m_HowToScene = new BitmapClass;
	if (!m_HowToScene)
	{
		return false;
	}
	//게임방법 씬 객체 초기화
	if (!m_HowToScene->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/HowToScene.png", 1920, 1080))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(map) object.", L"Error", MB_OK);
		return false;
	}

	//커서 객체 생성
	m_Cursor = new BitmapClass;
	if (!m_Cursor)
	{
		return false;
	}
	//커서 객체 초기화
	if (!m_Cursor->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Cursor.png", 80, 80))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(map) object.", L"Error", MB_OK);
		return false;
	}
	m_Cursor->SetNextPosX(500);
	m_Cursor->SetNextPosY(560);

	//셀 선택기 객체 생성
	m_CellSelector = new BitmapClass;
	if (!m_CellSelector)
	{
		return false;
	}
	//셀 선택기 객체 초기화
	if (!m_CellSelector->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/CellSelector.png", 80, 80))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(map) object.", L"Error", MB_OK);
		return false;
	}
	m_CellSelector->SetNextPosX(80);
	m_CellSelector->SetNextPosY(140);
	

	//배 객체 및 배열 초기화
	m_ShipArray = new BitmapClass*[10];
	//////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
	m_Ship2_H_num1 = new BitmapClass;
	m_ShipArray[0] = m_Ship2_H_num1;
	if (!m_Ship2_H_num1)
	{
		return false;
	}
	if (!m_Ship2_H_num1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Ship2_H.png", 160, 80))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(ship) object.", L"Error", MB_OK);
		return false;
	}

	m_Ship2_H_num2 = new BitmapClass;
	m_ShipArray[1] = m_Ship2_H_num2;
	if (!m_Ship2_H_num2)
	{
		return false;
	}
	if (!m_Ship2_H_num2->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Ship2_H.png", 160, 80))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(ship) object.", L"Error", MB_OK);
		return false;
	}

	m_Ship3_H_num1 = new BitmapClass;
	m_ShipArray[2] = m_Ship3_H_num1;
	if (!m_Ship3_H_num1)
	{
		return false;
	}
	if (!m_Ship3_H_num1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Ship3_H.png", 240, 80))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(ship) object.", L"Error", MB_OK);
		return false;
	}

	m_Ship3_H_num2 = new BitmapClass;
	m_ShipArray[3] = m_Ship3_H_num2;
	if (!m_Ship3_H_num2)
	{
		return false;
	}
	if (!m_Ship3_H_num2->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Ship3_H.png", 240, 80))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(ship) object.", L"Error", MB_OK);
		return false;
	}

	m_Ship4_H_num1 = new BitmapClass;
	m_ShipArray[4] = m_Ship4_H_num1;
	if (!m_Ship4_H_num1)
	{
		return false;
	}
	if (!m_Ship4_H_num1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Ship4_H.png", 320, 80))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(ship) object.", L"Error", MB_OK);
		return false;
	}

	m_Ship2_V_num1 = new BitmapClass;
	m_ShipArray[5] = m_Ship2_V_num1;
	if (!m_Ship2_V_num1)
	{
		return false;
	}
	if (!m_Ship2_V_num1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Ship2_V.png", 80, 160))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(ship) object.", L"Error", MB_OK);
		return false;
	}

	m_Ship2_V_num2 = new BitmapClass;
	m_ShipArray[6] = m_Ship2_V_num2;
	if (!m_Ship2_V_num2)
	{
		return false;
	}
	if (!m_Ship2_V_num2->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Ship2_V.png", 80, 160))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(ship) object.", L"Error", MB_OK);
		return false;
	}

	m_Ship3_V_num1 = new BitmapClass;
	m_ShipArray[7] = m_Ship3_V_num1;
	if (!m_Ship3_V_num1)
	{
		return false;
	}
	if (!m_Ship3_V_num1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Ship3_V.png", 80, 240))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(ship) object.", L"Error", MB_OK);
		return false;
	}

	m_Ship3_V_num2 = new BitmapClass;
	m_ShipArray[8] = m_Ship3_V_num2;
	if (!m_Ship3_V_num2)
	{
		return false;
	}
	if (!m_Ship3_V_num2->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Ship3_V.png", 80, 240))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(ship) object.", L"Error", MB_OK);
		return false;
	}

	m_Ship4_V_num1 = new BitmapClass;
	m_ShipArray[9] = m_Ship4_V_num1;
	if (!m_Ship4_V_num1)
	{
		return false;
	}
	if (!m_Ship4_V_num1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Ship4_V.png", 80, 320))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(ship) object.", L"Error", MB_OK);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////

	//승리 객체 생성
	WinScene = new BitmapClass;
	if (!WinScene)
	{
		return false;
	}
	if (!WinScene->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Win.png", 1920, 1080))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(ship) object.", L"Error", MB_OK);
		return false;
	}

	//패배 객체 생성
	LoseScene = new BitmapClass;
	if (!LoseScene)
	{
		return false;
	}
	if (!LoseScene->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, (WCHAR*)L"../DirectXEngine/data/Lose.png", 1920, 1080))
	{
		MessageBox(hwnd, L"Could not initialize the bitmap(ship) object.", L"Error", MB_OK);
		return false;
	}


	//함선의 기본 위치 설정
	for (int i = 0; i < 5; ++i)
	{
		m_ShipArray[i]->SetNextPosX(i*100);
	}

	for (int i = 5; i < 10; ++i)
	{
		m_ShipArray[i]->SetNextPosX(-9999);
	}

	//MyMapIsEnable,EnemyMapIsEnable 배열을 초기화합니다.
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			m_MyMapCellIsEnable[i][j] = false;
			m_EnemyMapCellIsEnable[i][j] = false;
			m_MyMapCellIsAttacked[i][j] = false;
			m_EnemyMapCellIsAttacked[i][j] = false;
			m_EnemyShipLocation[i][j] = 0;
			m_MyShipLocation[i][j] = 0;
		}
	}

	return true;
}


void GraphicsClass::Shutdown()
{

	//m_BackGround 객체 반환
	if (m_BackGround)
	{
		m_BackGround->Shutdown();
		delete m_BackGround;
		m_BackGround = 0;
	}

	//m_MyMap 객체 반환
	if (m_MyMap)
	{
		m_MyMap->Shutdown();
		delete m_MyMap;
		m_MyMap = 0;
	}

	//m_MyMap 객체 반환
	if (m_EnemyMap)
	{
		m_EnemyMap->Shutdown();
		delete m_EnemyMap;
		m_EnemyMap = 0;
	}

	//m_Intro 객체 반환
	if (m_Intro)
	{
		m_Intro->Shutdown();
		delete m_Intro;
		m_Intro = 0;
	}


	// m_TextureShader 객체 반환
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// m_Camera 객체 반환
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Direct3D 객체 반환
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}

bool GraphicsClass::Frame()
{
	//매 프레임이 시작될 때 마다 이전 destBitmapObjects 배열을 비워줍니다.
	for (int i = 0; i < m_BitmapArrayLength; ++i)
	{
		m_DestBitmapObjects[i] = nullptr;
	}

	//k는 DestbitmapObjects 배열의 인덱스로 사용합니다. l은 함선을 배치할 때 인덱스로 사용합니다.
	int k = 0;

	static float rotation = 0.0f;

	// 각 프레임의 rotation 변수를 업데이트합니다.
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	//게임 상태에 따라 렌더링할 객체를 바꿉니다 렌더링할 객체는 m_DestBitmapObjects 배열에 넣습니다 먼저 넣은 객체가 뒤에 렌더링됩니다.
	switch (GameStateClass::GetGameStateEnum())
	{
	case GameStateEnum::INTRO:
		m_DestBitmapObjects[0] = m_Intro;
		if (InputClass::GetInputObject()->IsAnyKeyDown())
		{
			GameStateClass::SetGameStateEnum(GameStateEnum::MAIN);
		}
		break;

	case GameStateEnum::HOWTOSCENE:
		m_DestBitmapObjects[0] = m_HowToScene;
		if (InputClass::GetInputObject()->IsKeyDownOnce(VK_BACK))
		{
			GameStateClass::SetGameStateEnum(GameStateEnum::MAIN);
		}
		break;

	case GameStateEnum::INTRODUCTIONSCENE:
		m_DestBitmapObjects[0] = m_IntroductionScene;
		if (InputClass::GetInputObject()->IsKeyDownOnce(VK_BACK))
		{
			GameStateClass::SetGameStateEnum(GameStateEnum::MAIN);
		}
		break;


	case GameStateEnum::MAIN:
		m_DestBitmapObjects[0] = m_MainScene;
		m_DestBitmapObjects[1] = m_Cursor;
		if (InputClass::GetInputObject()->IsKeyDownOnce(VK_DOWN))
		{
			AddCursorPositionY();
		}
		else if (InputClass::GetInputObject()->IsKeyDownOnce(VK_UP))
		{
			SubtractCursorPositionY();
		}
		m_Cursor->SetNextPosY(m_CursorPositionY);

		if (InputClass::GetInputObject()->IsKeyDownOnce(VK_RETURN))
		{
			switch (m_CursorDest)
			{
			case 0: GameStateClass::SetGameStateEnum(GameStateEnum::GAMEREADY);
				break;
			case 1: GameStateClass::SetGameStateEnum(GameStateEnum::HOWTOSCENE);
				break;
			case 2: GameStateClass::SetGameStateEnum(GameStateEnum::INTRODUCTIONSCENE);
				break;
			}
		}
		break;


	case GameStateEnum::GAMEREADY:
		
		m_DestBitmapObjects[0] = m_BackGround;
		m_DestBitmapObjects[1] = m_MyMap;
		m_DestBitmapObjects[2] = m_EnemyMap;
		k = 3;
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				m_DestBitmapObjects[k] = m_MyCellArray[i][j];
				m_DestBitmapObjects[k + 100] = m_EnemyCellArray[i][j];
				++k;
			}
		}


		k += 100;
		for (int i = 0; i < 10; ++i)
		{
			m_DestBitmapObjects[k] = m_ShipArray[i];
			++k;
		}
		m_DestBitmapObjects[k] = m_CellSelector;
		++k;

		if (InputClass::GetInputObject()->IsKeyDownOnce(VK_UP))
		{
			MoveCellSelector(VK_UP,true);
		}
		else if (InputClass::GetInputObject()->IsKeyDownOnce(VK_DOWN))
		{
			MoveCellSelector(VK_DOWN, true);
		}
		else if (InputClass::GetInputObject()->IsKeyDownOnce(VK_LEFT))
		{
			MoveCellSelector(VK_LEFT, true);
		}
		else if (InputClass::GetInputObject()->IsKeyDownOnce(VK_RIGHT))
		{
			MoveCellSelector(VK_RIGHT, true);
		}
		else if (InputClass::GetInputObject()->IsKeyDownOnce(VK_RETURN))
		{
			int indexXLength, indexYLength;
			indexXLength = m_ShipArray[l]->GetBitmapWidth()/80;
			indexYLength = m_ShipArray[l]->GetBitmapHeight()/80;
			
			for (int i = 0; i < indexXLength; ++i)
			{
				for (int j = 0; j < indexYLength; ++j)
				{
					if (m_MyMapCellIsEnable[m_CellSelectorXIndex + i][m_CellSelectorYIndex + j])
					{
						//스위치문을 탈출합니다.
						goto SwitchBreak;
					}
				}
			}

			for (int i = 0; i < indexXLength; ++i)
			{
				for (int j = 0; j < indexYLength; ++j)
				{
					m_MyMapCellIsEnable[m_CellSelectorXIndex + i][m_CellSelectorYIndex + j] = true;
					m_MyShipLocation[m_CellSelectorXIndex + i][m_CellSelectorYIndex + j] = l + 1;
				}
			}

			++l;
		}
		else if (InputClass::GetInputObject()->IsKeyDownOnce(VK_SPACE))
		{
			BitmapClass* temp = m_ShipArray[l];
			m_ShipArray[l] = m_ShipArray[l + 5];
			m_ShipArray[l + 5] = temp;
			m_ShipArray[l + 5]->SetNextPosX(-9999);

			if (m_CellSelector->GetNextPosY() > 860 + 80 - m_ShipArray[l]->GetBitmapHeight())
			{
				m_CellSelector->SetNextPosY(860 + 80 - m_ShipArray[l]->GetBitmapHeight());
			}

			if (m_CellSelector->GetNextPosX() > 800 + 80 - m_ShipArray[l]->GetBitmapWidth())
			{
				m_CellSelector->SetNextPosX(800 + 80 - m_ShipArray[l]->GetBitmapWidth());
			}

			m_CellSelectorXIndex = (m_CellSelector->GetNextPosX()-80)/80;
			m_CellSelectorYIndex = (m_CellSelector->GetNextPosY()-140)/80;
		}

		//현재 선택된 함선이 cell selector을 따라가게 합니다.
		m_ShipArray[l]->SetNextPosX(m_CellSelector->GetNextPosX());
		m_ShipArray[l]->SetNextPosY(m_CellSelector->GetNextPosY());
		
		//배치가 끝나면 다음 상태로 넘어갑니다.
		if (l > 4)
		{
			m_ShipArray[5]->SetNextPosX(-9999);
			GameStateClass::SetGameStateEnum(GameStateEnum::ENEMYREADY);
			
		}

		break;



	case GameStateEnum::ENEMYREADY:
		m_DestBitmapObjects[0] = m_BackGround;
		m_DestBitmapObjects[1] = m_MyMap;
		m_DestBitmapObjects[2] = m_EnemyMap;
		k = 3;
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				m_DestBitmapObjects[k] = m_MyCellArray[i][j];
				m_DestBitmapObjects[k + 100] = m_EnemyCellArray[i][j];
				++k;
			}
		}

		k += 100;
		for (int i = 0; i < 10; ++i)
		{
			m_DestBitmapObjects[k] = m_ShipArray[i];
			++k;
		}


		srand(time(NULL));
		//AI가 함선을 배치합니다.
		//i/2가 2,2,3,3,4 가 되는 for문
		for (int i = 4; i < 9; ++i)
		{
			int ShipLength = i / 2;
			int VerticalOrHorizontal = rand() % 2;
			//vertical
			if (VerticalOrHorizontal)
			{
				int x = rand() % 10;
				int y = rand() % (10 - ShipLength + 1);
				bool isAnything = false;
				
				for (int i = 0; i < ShipLength; ++i)
				{
					if (m_EnemyMapCellIsEnable[x][y+i])
					{
						isAnything = true;
					}
				}
				if (isAnything)
				{
					--i;
					continue;
				}
				else
				{
					for (int i = 0; i < ShipLength; ++i)
					{
						m_EnemyMapCellIsEnable[x][y + i] = true;
						m_EnemyShipLocation[x][y + i] = i - 4 + 1;
					}

				}
			}
			//horizontal
			else
			{
				int x = rand() % (10 - ShipLength + 1);
				int y = rand() % 10;
				bool isAnything = false;

				for (int i = 0; i < ShipLength; ++i)
				{
					if (m_EnemyMapCellIsEnable[x + i][y])
					{
						isAnything = true;
					}
				}
				if (isAnything)
				{
					--i;
					continue;
				}
				else
				{
					for (int i = 0; i < ShipLength; ++i)
					{
						m_EnemyMapCellIsEnable[x + i][y] = true;
						m_EnemyShipLocation[x + i][y] = i - 4 + 1;
					}
				}
			}

		}

		GameStateClass::SetGameStateEnum(GameStateEnum::MYTURN);
		m_CellSelector->SetNextPosX(1040);
		m_CellSelector->SetNextPosY(140);

		break;
	case GameStateEnum::MYTURN:
		m_DestBitmapObjects[0] = m_BackGround;
		m_DestBitmapObjects[1] = m_MyMap;
		m_DestBitmapObjects[2] = m_EnemyMap;
		k = 3;
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				m_DestBitmapObjects[k] = m_MyCellArray[i][j];
				m_DestBitmapObjects[k + 100] = m_EnemyCellArray[i][j];
				++k;
			}
		}

		k += 100;
		for (int i = 0; i < 10; ++i)
		{
			m_DestBitmapObjects[k] = m_ShipArray[i];
			++k;
		}

		m_DestBitmapObjects[k] = m_CellSelector;
		++k;

		if (InputClass::GetInputObject()->IsKeyDownOnce(VK_UP))
		{
			MoveCellSelector(VK_UP, false);
		}
		else if (InputClass::GetInputObject()->IsKeyDownOnce(VK_DOWN))
		{
			MoveCellSelector(VK_DOWN, false);
		}
		else if (InputClass::GetInputObject()->IsKeyDownOnce(VK_LEFT))
		{
			MoveCellSelector(VK_LEFT, false);
		}
		else if (InputClass::GetInputObject()->IsKeyDownOnce(VK_RIGHT))
		{
			MoveCellSelector(VK_RIGHT, false);
		}
		else if (InputClass::GetInputObject()->IsKeyDownOnce(VK_RETURN))
		{
			if (m_EnemyMapCellIsAttacked[m_CellSelectorXIndex][m_CellSelectorYIndex])
			{
				break;
			}
			
			m_CellSelectorXIndex = (m_CellSelector->GetNextPosX() - 1040) / 80;
			m_CellSelectorYIndex = (m_CellSelector->GetNextPosY() - 140) / 80;

			m_EnemyMapCellIsAttacked[m_CellSelectorXIndex][m_CellSelectorYIndex] = true;
			
			if (m_EnemyMapCellIsEnable[m_CellSelectorXIndex][m_CellSelectorYIndex])
			{
				m_EnemyCellArray[m_CellSelectorXIndex][m_CellSelectorYIndex]->SetTextureUV(0.33333f, 0.66666f, 0.0f, 1.0f);
			}
			else
			{
				m_EnemyCellArray[m_CellSelectorXIndex][m_CellSelectorYIndex]->SetTextureUV(0.66666f, 1.0f, 0.0f, 1.0f);
			}


			int temp = m_EnemyShipLocation[m_CellSelectorXIndex][m_CellSelectorYIndex];
			for (int i = 0; i < 10; ++i)
			{
				for (int j = 0; j < 10; ++j)
				{
					if (m_EnemyShipLocation[i][j] == temp)
					{
						m_EnemyShipLocation[i][j] = 0;
					}
				}
			}

			bool isWin = true;
			for (int i = 0; i < 10; ++i)
			{
				for (int j = 0; j < 10; ++j)
				{
					if (!m_EnemyShipLocation[i][j] == 0)
					{
						isWin = false;
						i = 100;
						break;
					}
				}
			}

			if (!isWin)
			{
				GameStateClass::SetGameStateEnum(GameStateEnum::ENEMYTURN);
			}
			else
			{
				winOrLose = true;
				GameStateClass::SetGameStateEnum(GameStateEnum::GAMEEND);
			}

		}

		break;

	case GameStateEnum::ENEMYTURN:
		m_DestBitmapObjects[0] = m_BackGround;
		m_DestBitmapObjects[1] = m_MyMap;
		m_DestBitmapObjects[2] = m_EnemyMap;
		k = 3;
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				m_DestBitmapObjects[k] = m_MyCellArray[i][j];
				m_DestBitmapObjects[k + 100] = m_EnemyCellArray[i][j];
				++k;
			}
		}

		k += 100;
		for (int i = 0; i < 10; ++i)
		{
			m_DestBitmapObjects[k] = m_ShipArray[i];
			++k;
		}

		int x;
		int y;
		srand(time(NULL));
		while(1)
		{
			x = rand() % 10;
			y = rand() % 10;

			if (!m_MyMapCellIsAttacked[x][y])
			{
				m_MyMapCellIsAttacked[x][y] = true;
				if (m_MyMapCellIsEnable[x][y])
				{
					m_MyCellArray[x][y]->SetTextureUV(0.33333f,0.66666f,0.0f,1.0f);
				}
				else
				{
					m_MyCellArray[x][y]->SetTextureUV(0.66666f, 1.0f,0.0f, 1.0f);
				}
				break;
			}
		}

		temp = m_MyShipLocation[x][y];
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				if (m_MyShipLocation[i][j] == temp)
				{
					m_MyShipLocation[i][j] = 0;
				}
			}
		}


		IsWin = true;
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				if (!m_MyShipLocation[i][j] == 0)
				{
					IsWin = false;
					i = 100;
					break;
				}
			}
		}

		if (!IsWin)
		{
			GameStateClass::SetGameStateEnum(GameStateEnum::MYTURN);
		}
		else
		{
			winOrLose = false;
			GameStateClass::SetGameStateEnum(GameStateEnum::GAMEEND);
		}
		break;

	case GameStateEnum::GAMEEND:
		if (IsWin)
		{
			m_DestBitmapObjects[0] = WinScene;
		}
		else
		{
			m_DestBitmapObjects[0] = LoseScene;
		}
		
		break;
	}

	SwitchBreak:

	// 그래픽 랜더링 처리
	return Render(rotation);
}


bool GraphicsClass::Render(float rotation)
{
	// 씬을 그리기 위해 버퍼를 지웁니다
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
	m_Direct3D->TurnZBufferOff();

	//비트맵 (백그라운드) 비트 맵 버텍스와 인덱스 버퍼를 그래픽 파이프라인에 배치하여 그리기를 준비합니다.
	//if (!m_BackGround->Render(m_Direct3D->GetDeviceContext(), 0, 0))
	//{
	//	return false;
	//}

	//// 텍스처 쉐이더로 비트 맵(백그라운드)을 렌더링합니다.	
	//if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_BackGround->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_BackGround->GetTexture()))
	//{
	//	return false;
	//}

	//// 비트 맵 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
	//if (!m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 100, 100))
	//{
	//	return false;
	//}

	//// 텍스처 쉐이더로 비트 맵을 렌더링합니다.	
	//if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture()))
	//{
	//	return false;
	//}

	//m_DestBitmapObject 안에 들어있는 모든 오브젝트를 렌더링합니다.
	for (int i = 0; i < m_BitmapArrayLength; ++i)
	{
		// 비트 맵 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
		if (m_DestBitmapObjects[i])
		{
			if (!m_DestBitmapObjects[i]->Render(m_Direct3D->GetDeviceContext(), m_DestBitmapObjects[i]->GetNextPosX(), m_DestBitmapObjects[i]->GetNextPosY()))
			{
				return false;
			}

			// 텍스처 쉐이더로 비트 맵을 렌더링합니다.	
			if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DestBitmapObjects[i]->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_DestBitmapObjects[i]->GetTexture()))
			{
				return false;
			}
		}
	}


	// 모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
	m_Direct3D->TurnZBufferOn();

	// 버퍼의 내용을 화면에 출력합니다
	m_Direct3D->EndScene();

	return true;
}

void GraphicsClass::AddCursorPositionY()
{
	m_CursorPositionY += 165;
	if (m_CursorPositionY > 890)
	{
		m_CursorPositionY = 890;
	}

	m_CursorDest += 1;
	if (m_CursorDest > 2)
	{
		m_CursorDest = 2;
	}
}

void GraphicsClass::SubtractCursorPositionY()
{
	m_CursorPositionY -= 165;
	if (m_CursorPositionY < 560)
	{
		m_CursorPositionY = 560;
	}
	m_CursorDest -= 1;
	if (m_CursorDest < 0)
	{
		m_CursorDest = 0;
	}
}

void GraphicsClass::MoveCellSelector(int key, bool IsMyMap)
{
	int RightEnd, LeftEnd, UpEnd, DownEnd;

	UpEnd = 140;
	DownEnd = 860;

	//트루면 내맵 아니면 적맵
	if (IsMyMap)
	{
		RightEnd = 800;
		LeftEnd = 80;
		switch (key)
		{
		case VK_LEFT:
			m_CellSelector->SetNextPosX(m_CellSelector->GetPreviousPosX() - 80);
			--m_CellSelectorXIndex;
			if (m_CellSelector->GetNextPosX() < LeftEnd)
			{
				m_CellSelector->SetNextPosX(LeftEnd);
				m_CellSelectorXIndex = 0;
			}
			break;
		case VK_RIGHT:
			m_CellSelector->SetNextPosX(m_CellSelector->GetPreviousPosX() + 80);
			++m_CellSelectorXIndex;
			if (m_CellSelector->GetNextPosX() > RightEnd +80 - m_ShipArray[l]->GetBitmapWidth())
			{
				m_CellSelector->SetNextPosX(RightEnd+ 80 - m_ShipArray[l]->GetBitmapWidth());
				m_CellSelectorXIndex = 9;
			}
			break;
		case VK_UP:
			m_CellSelector->SetNextPosY(m_CellSelector->GetPreviousPosY() - 80);
			--m_CellSelectorYIndex;
			if (m_CellSelector->GetNextPosY() < UpEnd)
			{
				m_CellSelector->SetNextPosY(UpEnd);
				m_CellSelectorYIndex = 0;
			}
			break;
		case VK_DOWN:
			m_CellSelector->SetNextPosY(m_CellSelector->GetPreviousPosY() + 80);
			++m_CellSelectorYIndex;
			if (m_CellSelector->GetNextPosY() > DownEnd + 80 - m_ShipArray[l]->GetBitmapHeight())
			{
				m_CellSelector->SetNextPosY(DownEnd + 80 - m_ShipArray[l]->GetBitmapHeight());
				m_CellSelectorYIndex = 9;
			}
			break;
		}
	}
	else
	{
		RightEnd = 1760;
		LeftEnd = 1040;
		switch (key)
		{
		case VK_LEFT:
			m_CellSelector->SetNextPosX(m_CellSelector->GetPreviousPosX() - 80);
			--m_CellSelectorXIndex;
			if (m_CellSelector->GetNextPosX() < LeftEnd)
			{
				m_CellSelector->SetNextPosX(LeftEnd);
				m_CellSelectorXIndex = 0;
			}
			break;
		case VK_RIGHT:
			m_CellSelector->SetNextPosX(m_CellSelector->GetPreviousPosX() + 80);
			++m_CellSelectorXIndex;
			if (m_CellSelector->GetNextPosX() > RightEnd)
			{
				m_CellSelector->SetNextPosX(RightEnd);
				m_CellSelectorXIndex = 9;
			}
			break;
		case VK_UP:
			m_CellSelector->SetNextPosY(m_CellSelector->GetPreviousPosY() - 80);
			--m_CellSelectorYIndex;
			if (m_CellSelector->GetNextPosY() < UpEnd)
			{
				m_CellSelector->SetNextPosY(UpEnd);
				m_CellSelectorYIndex = 0;
			}
			break;
		case VK_DOWN:
			m_CellSelector->SetNextPosY(m_CellSelector->GetPreviousPosY() + 80);
			++m_CellSelectorYIndex;
			if (m_CellSelector->GetNextPosY() > DownEnd)
			{
				m_CellSelector->SetNextPosY(DownEnd);
				m_CellSelectorYIndex = 9;
			}
			break;
		}
	}
}