/***************************************************************************
 # Copyright (c) 2018, NVIDIA CORPORATION. All rights reserved.
 #
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted provided that the following conditions
 # are met:
 #  * Redistributions of source code must retain the above copyright
 #    notice, this list of conditions and the following disclaimer.
 #  * Redistributions in binary form must reproduce the above copyright
 #    notice, this list of conditions and the following disclaimer in the
 #    documentation and/or other materials provided with the distribution.
 #  * Neither the name of NVIDIA CORPORATION nor the names of its
 #    contributors may be used to endorse or promote products derived
 #    from this software without specific prior written permission.
 #
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 # EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 # IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 # PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 # CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 # EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 # PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 # PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 # OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 # (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 # OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************/
#pragma once
#include "Framework.h"
#include "ImageDataHandler.h"

class Tutorial14 : public Tutorial
{
public:
    struct AccelerationStructureBuffers
    {
        ID3D12ResourcePtr pScratch;
        ID3D12ResourcePtr pResult;
        ID3D12ResourcePtr pInstanceDesc;    // Used only for top-level AS
    };


    void onLoad(HWND winHandle, uint32_t winWidth, uint32_t winHeight) override;
	void onFrameRender() override;
	void extractImageDataFromSwapchain();
    void onShutdown() override;

private:
    //////////////////////////////////////////////////////////////////////////
    // Tutorial 02 code
    //////////////////////////////////////////////////////////////////////////
    void initDXR(HWND winHandle, uint32_t winWidth, uint32_t winHeight);
    uint32_t beginFrame();
    void endFrame(uint32_t rtvIndex);
    HWND mHwnd = nullptr;
    ID3D12Device5Ptr mpDevice;
    ID3D12CommandQueuePtr mpCmdQueue;
    IDXGISwapChain3Ptr mpSwapChain;
    uvec2 mSwapChainSize;
    ID3D12GraphicsCommandList4Ptr mpCmdList;
    ID3D12FencePtr mpFence;
    HANDLE mFenceEvent;
    uint64_t mFenceValue = 0;
	ID3D12ResourcePtr dstResource;

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
    ID3D12ResourcePtr mpBottomLevelAS[2];
    AccelerationStructureBuffers mTopLevelBuffers;
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
    void createShaderResources();
    ID3D12ResourcePtr mpOutputResource;
    ID3D12DescriptorHeapPtr mpSrvUavHeap;
    static const uint32_t kSrvUavHeapSize = 2;

    //////////////////////////////////////////////////////////////////////////
    // Tutorial 10
    //////////////////////////////////////////////////////////////////////////
    void createConstantBuffers();
    ID3D12ResourcePtr mpConstantBuffer[3];
    
    //////////////////////////////////////////////////////////////////////////
    // Tutorial 14
    //////////////////////////////////////////////////////////////////////////
    float mRotation = 0;
};
