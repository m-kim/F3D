#include "vtkF3DInteractorStyle.h"

#include "F3DLoader.h"
#include "vtkF3DRenderer.h"

#include <vtkCallbackCommand.h>
#include <vtkMath.h>
#include <vtkMatrix3x3.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>
#include <vtkStringArray.h>

vtkStandardNewMacro(vtkF3DInteractorStyle);

//----------------------------------------------------------------------------
void vtkF3DInteractorStyle::OnDropFiles(vtkStringArray* files)
{
  vtkRenderWindowInteractor* rwi = this->GetInteractor();
  vtkRenderWindow* renWin = rwi->GetRenderWindow();
  this->InvokeEvent(F3DLoader::NewFilesEvent, files);
  renWin->Render();
}

//----------------------------------------------------------------------------
void vtkF3DInteractorStyle::OnKeyPress()
{
  vtkRenderWindowInteractor* rwi = this->GetInteractor();
  vtkRenderWindow* renWin = rwi->GetRenderWindow();
  vtkF3DRenderer* ren = vtkF3DRenderer::SafeDownCast(renWin->GetRenderers()->GetFirstRenderer());

  switch (rwi->GetKeyCode())
  {
    case 's':
    case 'S':
      ren->ShowScalars(!ren->AreScalarsVisible());
      renWin->Render();
      break;
    case 'b':
    case 'B':
      ren->ShowScalarBar(!ren->IsScalarBarVisible());
      renWin->Render();
      break;
    case 'p':
    case 'P':
      ren->SetUseDepthPeelingPass(!ren->UsingDepthPeelingPass());
      renWin->Render();
      break;
    case 'q':
    case 'Q':
      ren->SetUseSSAOPass(!ren->UsingSSAOPass());
      renWin->Render();
      break;
    case 'a':
    case 'A':
      ren->SetUseFXAAPass(!ren->UsingFXAAPass());
      renWin->Render();
      break;
    case 't':
    case 'T':
      ren->SetUseToneMappingPass(!ren->UsingToneMappingPass());
      renWin->Render();
      break;
    case 'e':
    case 'E':
      ren->ShowEdge(!ren->IsEdgeVisible());
      renWin->Render();
      break;
    case 'x':
    case 'X':
      ren->ShowAxis(!ren->IsAxisVisible());
      renWin->Render();
      break;
    case 'g':
    case 'G':
      ren->ShowGrid(!ren->IsGridVisible());
      renWin->Render();
      break;
    case 'n':
    case 'N':
      ren->ShowFilename(!ren->IsFilenameVisible());
      renWin->Render();
      break;
    case 'm':
    case 'M':
      ren->ShowMetaData(!ren->IsMetaDataVisible());
      renWin->Render();
      break;
    case 'z':
    case 'Z':
      ren->ShowTimer(!ren->IsTimerVisible());
      renWin->Render();
      if (ren->IsTimerVisible())
      {
        // Needed to show a correct value, computed at the previous render
        // TODO: Improve this by computing it and displaying it within a single render
        renWin->Render();
      }
      break;
    case 'r':
    case 'R':
      ren->SetUseRaytracing(!ren->UsingRaytracing());
      renWin->Render();
      break;
    case 'd':
    case 'D':
      ren->SetUseRaytracingDenoiser(!ren->UsingRaytracingDenoiser());
      renWin->Render();
      break;
    case 'v':
    case 'V':
      ren->SetUseVolume(!ren->UsingVolume());
      renWin->Render();
      break;
    case 'i':
    case 'I':
      ren->SetUseInverseOpacityFunction(!ren->UsingInverseOpacityFunction());
      renWin->Render();
      break;
    case 'o':
    case 'O':
      ren->SetUsePointSprites(!ren->UsingPointSprites());
      renWin->Render();
      break;
    case 'f':
    case 'F':
      renWin->SetFullScreen(!renWin->GetFullScreen());

      // when going full screen, the OpenGL context changes, we need to reinitialize
      // the interactor, the render passes and the grid actor.
      ren->ShowGrid(ren->IsGridVisible());
      ren->SetupRenderPasses();
      rwi->ReInitialize();

      renWin->Render();
      break;
    case 'u':
    case 'U':
      ren->SetUseBlurBackground(!ren->UsingBlurBackground());
      renWin->Render();
      break;
    case 'h':
    case 'H':
      ren->ShowCheatSheet(!ren->IsCheatSheetVisible());
      renWin->Render();
      break;
    case '?':
      ren->DumpSceneState();
      break;
    default:
      std::string keySym = rwi->GetKeySym();
      if (keySym == "Left")
      {
        int load = F3DLoader::LOAD_PREVIOUS;
        this->InvokeEvent(F3DLoader::LoadFileEvent, &load);
        renWin->Render();
      }
      else if (keySym == "Right")
      {
        int load = F3DLoader::LOAD_NEXT;
        this->InvokeEvent(F3DLoader::LoadFileEvent, &load);
        renWin->Render();
      }
      else if (keySym == "Escape")
      {
        rwi->ExitCallback();
      }
      else if (keySym == "Return")
      {
        ren->ResetCamera();
        renWin->Render();
      }
      break;
  }
}

//----------------------------------------------------------------------------
void vtkF3DInteractorStyle::EnvironmentRotate()
{
  this->Superclass::EnvironmentRotate();

  vtkF3DRenderer* ren = vtkF3DRenderer::SafeDownCast(this->CurrentRenderer);
  if (ren)
  {
    // update skybox orientation
    double* up = ren->GetEnvironmentUp();
    double* right = ren->GetEnvironmentRight();

    ren->GetSkybox()->SetFloorPlane(-up[2], up[1], up[0], 0.0);
    ren->GetSkybox()->SetFloorRight(-right[2], right[1], right[0]);

    this->Interactor->Render();
  }
}
