#pragma once

#include "scripting/graph/node.h"
#include <imgui_node_editor.h>
#include <vector>
#include <map>

namespace ed = ax::NodeEditor;

struct GraphEditor
{
    int GetNextId();

    // ed::NodeId GetNextNodeId()
    //{
    //     return ed::NodeId(GetNextId());
    // }

    ed::LinkId GetNextLinkId();

    void TouchNode(ed::NodeId id);

    float GetTouchProgress(ed::NodeId id);

    void UpdateTouch();

    Node* FindNode(ed::NodeId id);

    Link* FindLink(ed::LinkId id);

    Pin* FindPin(ed::PinId id);

    bool IsPinLinked(ed::PinId id);

    bool CanCreateLink(Pin* a, Pin* b);

    // void DrawItemRect(ImColor color, float expand = 0.0f)
    //{
    //     ImGui::GetWindowDrawList()->AddRect(
    //         ImGui::GetItemRectMin() - ImVec2(expand, expand),
    //         ImGui::GetItemRectMax() + ImVec2(expand, expand),
    //         color);
    // };

    // void FillItemRect(ImColor color, float expand = 0.0f, float rounding = 0.0f)
    //{
    //     ImGui::GetWindowDrawList()->AddRectFilled(
    //         ImGui::GetItemRectMin() - ImVec2(expand, expand),
    //         ImGui::GetItemRectMax() + ImVec2(expand, expand),
    //         color, rounding);
    // };

    void BuildNode(Node* node);

    Node* SpawnInputActionNode();

    Node* SpawnBranchNode();

    Node* SpawnDoNNode();

    Node* SpawnOutputActionNode();

    Node* SpawnPrintStringNode();

    Node* SpawnMessageNode();

    Node* SpawnSetTimerNode();

    Node* SpawnLessNode();

    Node* SpawnWeirdNode();

    Node* SpawnTraceByChannelNode();

    Node* SpawnTreeSequenceNode();

    Node* SpawnTreeTaskNode();

    Node* SpawnTreeTask2Node();

    Node* SpawnComment();

    Node* SpawnHoudiniTransformNode();

    Node* SpawnHoudiniGroupNode();

    void BuildNodes();

    void OnStart();

    void OnStop();

    ImColor GetIconColor(PinType type);
    ;

    void DrawPinIcon(const Pin& pin, bool connected, int alpha);
    ;

    void ShowStyleEditor(bool* show = nullptr);

    void ShowLeftPane(float paneWidth);

    void OnFrame(float deltaTime);

    int                                     m_NextId      = 1;
    const int                               m_PinIconSize = 24;
    std::vector<Node>                       m_Nodes;
    std::vector<Link>                       m_Links;
    ImTextureID                             m_HeaderBackground = nullptr;
    ImTextureID                             m_SaveIcon         = nullptr;
    ImTextureID                             m_RestoreIcon      = nullptr;
    const float                             m_TouchTime        = 1.0f;
    std::map<ed::NodeId, float, NodeIdLess> m_NodeTouchTime;
    bool                                    m_ShowOrdinals = false;
};
