#include "pch.h"
#include "CViewInit.h"
#include "CViewBakeLightmap.h"

#include "ViewManager/CViewManager.h"
#include "Context/CContext.h"

#include "GridPlane/CGridPlane.h"
#include "SkyDome/CSkyDome.h"

CViewInit::CViewInit() :
	m_initState(CViewInit::DownloadBundles),
	m_getFile(NULL),
	m_spriteArchive(NULL),
	m_downloaded(0)
{

}

CViewInit::~CViewInit()
{

}

io::path CViewInit::getBuiltInPath(const char* name)
{
	return getApplication()->getBuiltInPath(name);
}

void CViewInit::onInit()
{
	getApplication()->getFileSystem()->addFileArchive(getBuiltInPath("BuiltIn.zip"), false, false);

	CShaderManager* shaderMgr = CShaderManager::getInstance();
	shaderMgr->initBasicShader();
	shaderMgr->initSGDeferredShader();

	CGlyphFreetype* freetypeFont = CGlyphFreetype::getInstance();
	freetypeFont->initFont("Segoe UI Light", "BuiltIn/Fonts/segoeui/segoeuil.ttf");
}

void CViewInit::initScene()
{
	CBaseApp* app = getApplication();

	// show console
	app->showDebugConsole();

	CScene* scene = CContext::getInstance()->initScene();
	CZone* zone = scene->createZone();

	// camera
	CGameObject* camObj = zone->createEmptyObject();
	camObj->addComponent<CCamera>();
	camObj->addComponent<CEditorCamera>()->setMoveSpeed(2.0f);
	camObj->addComponent<CFpsMoveCamera>()->setMoveSpeed(1.0f);

	CCamera* camera = camObj->getComponent<CCamera>();
	camera->setPosition(core::vector3df(-10.0f, 5.0f, 10.0f));
	camera->lookAt(core::vector3df(0.0f, 0.0f, 0.0f), core::vector3df(0.0f, 1.0f, 0.0f));

	// gui camera
	CGameObject* guiCameraObj = zone->createEmptyObject();
	guiCameraObj->addComponent<CCamera>();
	CCamera* guiCamera = guiCameraObj->getComponent<CCamera>();
	guiCamera->setProjectionType(CCamera::OrthoUI);

	// sky
	ITexture* skyDomeTexture = CTextureManager::getInstance()->getTexture("Common/Textures/Sky/MonValley.png");
	if (skyDomeTexture != NULL)
	{
		CSkyDome* skyDome = zone->createEmptyObject()->addComponent<CSkyDome>();
		skyDome->setData(skyDomeTexture, SColor(255, 255, 255, 255));
	}

	// lighting
	CGameObject* lightObj = zone->createEmptyObject();
	CDirectionalLight* directionalLight = lightObj->addComponent<CDirectionalLight>();
	SColor c(255, 255, 244, 214);
	directionalLight->setColor(SColorf(c));

	CTransformEuler* lightTransform = lightObj->getTransformEuler();
	lightTransform->setPosition(core::vector3df(2.0f, 2.0f, 2.0f));

	core::vector3df direction = core::vector3df(0.0f, -1.5f, 2.0f);
	lightTransform->setOrientation(direction, CTransform::s_oy);

	CMeshManager* meshManager = CMeshManager::getInstance();
	CEntityPrefab* prefab = NULL;

	std::vector<std::string> textureFolders;

	// load model
	prefab = meshManager->loadModel("TankScene/TankScene.obj", NULL, true);
	if (prefab != NULL)
	{
		// export model material if TankScene.xml is not exist
		// CMaterialManager::getInstance()->exportMaterial(prefab, "../Assets/TankScene/TankScene.xml");

		// load material
		ArrayMaterial& materials = CMaterialManager::getInstance()->loadMaterial("TankScene/TankScene.xml", true, textureFolders);

		// todo modify material
		//for (CMaterial* m : materials)
		//{
		//	m->changeShader("BuiltIn/Shader/SpecularGlossiness/Deferred/Diffuse.xml");
		//	m->autoDetectLoadTexture();
		//	m->deleteExtramParams();
		//}

		// and save material
		// CMaterialManager::getInstance()->saveMaterial(materials, "../Assets/TankScene/TankScene1.xml");

		// create render mesh object
		CGameObject* tankScene = zone->createEmptyObject();
		tankScene->setStatic(true);

		CRenderMesh* renderer = tankScene->addComponent<CRenderMesh>();
		renderer->initFromPrefab(prefab);
		renderer->initMaterial(materials);

		CIndirectLighting* indirectLighting = tankScene->addComponent<CIndirectLighting>();
		indirectLighting->setIndirectLightingType(CIndirectLighting::VertexColor);
	}

	// save to context
	CContext* context = CContext::getInstance();
	context->initRenderPipeline(app->getWidth(), app->getHeight(), false);
	context->setActiveZone(zone);
	context->setActiveCamera(camera);
	context->setGUICamera(guiCamera);
	context->setDirectionalLight(directionalLight);
}

void CViewInit::onDestroy()
{

}

void CViewInit::onUpdate()
{
	CContext* context = CContext::getInstance();

	switch (m_initState)
	{
	case CViewInit::DownloadBundles:
	{
		io::IFileSystem* fileSystem = getApplication()->getFileSystem();


		std::vector<std::string> listBundles;
		listBundles.push_back("Common.zip");
		listBundles.push_back("TankScene.zip");
		listBundles.push_back("TankSceneResource.zip");
		listBundles.push_back(getApplication()->getTexturePackageName("TankScene"));

#ifdef __EMSCRIPTEN__
		const char* filename = listBundles[m_downloaded].c_str();

		if (m_getFile == NULL)
		{
			m_getFile = new CGetFileURL(filename, filename);
			m_getFile->download(CGetFileURL::Get);

			char log[512];
			sprintf(log, "Download asset: %s", filename);
			os::Printer::log(log);
		}
		else
		{
			if (m_getFile->getState() == CGetFileURL::Finish)
			{
				// [bundles].zip
				fileSystem->addFileArchive(filename, false, false);

				if (++m_downloaded >= listBundles.size())
					m_initState = CViewInit::InitScene;
				else
				{
					delete m_getFile;
					m_getFile = NULL;
				}
			}
			else if (m_getFile->getState() == CGetFileURL::Error)
			{
				// retry download
				delete m_getFile;
				m_getFile = NULL;
			}
		}
#else

		for (std::string& bundle : listBundles)
		{
			const char* r = bundle.c_str();
#if defined(WINDOWS_STORE)
			fileSystem->addFileArchive(getBuiltInPath(r), false, false);
#elif defined(MACOS)
			fileSystem->addFileArchive(getBuiltInPath(r), false, false);
#else
			fileSystem->addFileArchive(r, false, false);
#endif
		}

		m_initState = CViewInit::InitScene;
#endif
	}
	break;
	case CViewInit::InitScene:
	{
		if (m_spriteArchive != NULL)
		{
			m_sprite = new CSpriteAtlas(video::ECF_A8R8G8B8, 2048, 2048);

			// get list sprite image
			std::vector<std::string> sprites;

			const io::IFileList* fileList = m_spriteArchive->getFileList();
			for (int i = 0, n = fileList->getFileCount(); i < n; i++)
			{
				const char* fullFileame = fileList->getFullFileName(i).c_str();
				const char* name = fileList->getFileName(i).c_str();

				m_sprite->addFrame(name, fullFileame);
			}

			m_sprite->updateTexture();
		}

		initScene();
		m_initState = CViewInit::Finished;
	}
	break;
	case CViewInit::Error:
	{
		// todo nothing with black screen
	}
	break;
	default:
	{
		CScene* scene = context->getScene();
		if (scene != NULL)
			scene->update();

		CViewManager::getInstance()->getLayer(0)->changeView<CViewBakeLightmap>();
	}
	break;
	}
}

void CViewInit::onRender()
{

}
