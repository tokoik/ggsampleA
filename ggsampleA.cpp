﻿// Dear ImGui を使う
#define USE_IMGUI

// ウィンドウ関連の処理
#include "Window.h"

// ファイルダイアログ
#include "nfd.hpp"
// Windows のとき
#if defined(_MSC_VER)
// リンクするライブラリ
#  pragma comment(lib, "lib\\" GLFW3_PLATFORM "\\" GLFW3_CONFIGURATION "\\nfd.lib")
#endif

// 初期モデルデータ
static std::string model{ "logo.obj" };

// 光源データ
static GgSimpleShader::Light light
{
  { 0.2f, 0.2f, 0.2f, 1.0f }, // 環境光成分
  { 1.0f, 1.0f, 1.0f, 0.0f }, // 拡散反射光成分
  { 1.0f, 1.0f, 1.0f, 0.0f }, // 鏡面反射光成分
  { 0.5f, 0.5f, 1.0f, 1.0f }  // 光源位置
};

//
// アプリケーション本体
//
void app()
{
  // ウィンドウを作成する
  Window window{ "ggsampleA" };

  // 図形データを読み込む (大きさを正規化する)
  GgSimpleObj object{ model, true };

  // シェーダを作成する
  const GgSimpleShader simple{ "simple.vert", "simple.frag" };
  const GLint rotateLoc{ glGetUniformLocation(simple.get(), "rotate") };
  const GLint translateLoc{ glGetUniformLocation(simple.get(), "translate") };

  // 光源データから光源のバッファオブジェクトを作成する
  GgSimpleShader::LightBuffer lightBuffer{ light };

  // ビュー変換行列を設定する
  const GgMatrix mv{ ggLookat(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f) };

#ifdef USE_IMGUI
  //
  // ImGui の初期設定
  //

  // Native File Dialog Extended の初期化
  NFD_Init();

  //ImGuiIO& io = ImGui::GetIO();
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  //ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Read 'docs/FONTS.txt' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);
#endif

  // 背景色を設定する
  glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

  // 隠面消去処理を設定する
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef USE_IMGUI
    //
    // ImGui によるユーザインタフェース
    //

    // ImGui のフレームを準備する
    ImGui::NewFrame();

    //
    // メニューバー
    //
    if (ImGui::BeginMainMenuBar())
    {
      // File メニュー
      if (ImGui::BeginMenu("File"))
      {
        // モデルファイルを開く
        if (ImGui::MenuItem("Open Model File"))
        {
          // ファイルダイアログから得るパス
          nfdchar_t* filepath(NULL);

          // ファイルダイアログを開く
          const nfdfilteritem_t filter[]{ "Wavefront OBJ", "obj" };
          if (NFD_OpenDialog(&filepath, filter, 1, nullptr) == NFD_OKAY)
          {
            // 画像を読み込む
            object = GgSimpleObj(filepath, true);
          }

          // ファイルパスの取り出しに使ったメモリを開放する
          if (filepath) NFD_FreePath(filepath);
        }

        // 終了
        else if (ImGui::MenuItem("Quit")) window.setClose();

        // File メニュー修了
        ImGui::EndMenu();
      }

      // メインメニューバー終了
      ImGui::EndMainMenuBar();
    }

    //
    // 光源位置
    //
    ImGui::SetNextWindowPos(ImVec2(4, 24), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(400, 54), ImGuiCond_Once);
    ImGui::Begin("Contrl Panel");
    if (ImGui::SliderFloat3("Light Position", light.position.data(), -10.0f, 10.0f, "%.2f"))
      lightBuffer.loadPosition(light.position);
    ImGui::End();
#endif

    // 投影変換行列を設定する
    const GgMatrix mp{ ggPerspective(0.5f, window.getAspect(), 1.0f, 15.0f) };

    // シェーダプログラムを指定する
    simple.use(mp, mv, lightBuffer);

    // オブジェクトの回転を指定する
    glUniformMatrix4fv(rotateLoc, 1, GL_FALSE, window.getRotationMatrix(0).get());

    // オブジェクトの並進を指定する
    glUniformMatrix4fv(translateLoc, 1, GL_FALSE, window.getTranslationMatrix(1).get());

    // 図形を描画する
    object.draw();

#ifdef USE_IMGUI
    // ImGui のフレームに描画する
    ImGui::Render();
#endif

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }

#ifdef USE_IMGUI
  // Native File Dialog Extended 終了
  NFD_Quit();
#endif
}
