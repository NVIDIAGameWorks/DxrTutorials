
/************************************************************************************************************************************\
|*                                                                                                                                    *|
|*     Copyright © 2018 NVIDIA Corporation.  All rights reserved.                                                                     *|
|*                                                                                                                                    *|
|*  NOTICE TO USER:                                                                                                                   *|
|*                                                                                                                                    *|
|*  This software is subject to NVIDIA ownership rights under U.S. and international Copyright laws.                                  *|
|*                                                                                                                                    *|
|*  This software and the information contained herein are PROPRIETARY and CONFIDENTIAL to NVIDIA                                     *|
|*  and are being provided solely under the terms and conditions of an NVIDIA software license agreement                              *|
|*  and / or non-disclosure agreement.  Otherwise, you have no rights to use or access this software in any manner.                   *|
|*                                                                                                                                    *|
|*  If not covered by the applicable NVIDIA software license agreement:                                                               *|
|*  NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOFTWARE FOR ANY PURPOSE.                                            *|
|*  IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND.                                                           *|
|*  NVIDIA DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,                                                                     *|
|*  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.                       *|
|*  IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES,                               *|
|*  OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  WHETHER IN AN ACTION OF CONTRACT,                         *|
|*  NEGLIGENCE OR OTHER TORTIOUS ACTION,  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.            *|
|*                                                                                                                                    *|
|*  U.S. Government End Users.                                                                                                        *|
|*  This software is a "commercial item" as that term is defined at 48 C.F.R. 2.101 (OCT 1995),                                       *|
|*  consisting  of "commercial computer  software"  and "commercial computer software documentation"                                  *|
|*  as such terms are  used in 48 C.F.R. 12.212 (SEPT 1995) and is provided to the U.S. Government only as a commercial end item.     *|
|*  Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through 227.7202-4 (JUNE 1995),                                          *|
|*  all U.S. Government End Users acquire the software with only those rights set forth herein.                                       *|
|*                                                                                                                                    *|
|*  Any use of this software in individual and commercial software must include,                                                      *|
|*  in the user documentation and internal comments to the code,                                                                      *|
|*  the above Disclaimer (as applicable) and U.S. Government End Users Notice.                                                        *|
|*                                                                                                                                    *|
 \************************************************************************************************************************************/
#pragma once
#include "Falcor.h"

using namespace Falcor;

MAKE_SMART_COM_PTR(ID3D12Device5);
MAKE_SMART_COM_PTR(ID3D12GraphicsCommandList4);

class DxrSample : public Renderer
{
public:
    void onLoad(SampleCallbacks* pSample, const RenderContext::SharedPtr& pRenderContext) override;
    void onFrameRender(SampleCallbacks* pSample, const RenderContext::SharedPtr& pRenderContext, const Fbo::SharedPtr& pTargetFbo) override;
    void onShutdown(SampleCallbacks* pSample) override;
    bool onMouseEvent(SampleCallbacks* pSample, const MouseEvent& mouseEvent) override;
    bool onKeyEvent(SampleCallbacks* pSample, const KeyboardEvent& keyEvent) override;
private:
//////////////////////////////////////////////////////////////////////////
// Tutorial 02 code
//////////////////////////////////////////////////////////////////////////
    void initDXR(const Window* pWindow);
    uint32_t beginFrame();
    void endFrame(uint32_t rtvIndex);
    HWND mHwnd = nullptr;
    ID3D12Device5Ptr mpDevice;
    ID3D12CommandQueuePtr mpCmdQueue;
    IDXGISwapChain3Ptr mpSwapChain;
    ID3D12GraphicsCommandList4Ptr mpCmdList;
    ID3D12FencePtr mpFence;
    HANDLE mFenceEvent;
    uint64_t mFenceValue = 0;

    struct
    {
        ID3D12CommandAllocatorPtr pCmdAllocator;
        ID3D12ResourcePtr pSwapChainBuffer;
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
    } mFrameObjects[kDefaultSwapChainBuffers];


    // Heap data
    struct HeapData
    {
        ID3D12DescriptorHeapPtr pHeap;
        uint32_t usedEntries = 0;
    };
    HeapData mRtvHeap;
    static const uint32_t kRtvHeapSize = 3;

    //////////////////////////////////////////////////////////////////////////
    // Tutorial 03, Tutorial 11
    //////////////////////////////////////////////////////////////////////////
    void createAccelerationStructures();
    ID3D12ResourcePtr mpVertexBuffer[2];
    ID3D12ResourcePtr mpTopLevelAS;
    ID3D12ResourcePtr mpBottomLevelAS[2];
    uint64_t mTlasSize = 0;

    //////////////////////////////////////////////////////////////////////////
    // Tutorial 04
    //////////////////////////////////////////////////////////////////////////
    void createRtPipelineState();
    ID3D12StateObjectPtr mpPipelineState;
    ID3D12RootSignaturePtr mpEmptyRootSig;
    
    //////////////////////////////////////////////////////////////////////////
    // Tutorial 05
    //////////////////////////////////////////////////////////////////////////
    void createShaderTable();
    ID3D12ResourcePtr mpShaderTable;
    uint32_t mShaderTableEntrySize = 0;

    //////////////////////////////////////////////////////////////////////////
    // Tutorial 06
    //////////////////////////////////////////////////////////////////////////
    void createShaderResources(const Window* pWindow);
    ID3D12ResourcePtr mpOutputResource;
    ID3D12DescriptorHeapPtr mpSrvUavHeap;
    static const uint32_t kSrvUavHeapSize = 2;

    //////////////////////////////////////////////////////////////////////////
    // Tutorial 10
    //////////////////////////////////////////////////////////////////////////
    void createConstantBuffers();
    ID3D12ResourcePtr mpConstantBuffer[3];
};
