/*
 * DeckLinkOpenCv.cpp - Conversión de formato DeckLink a OpenCV
 *
 *   Copyright 2013 Jesús Torres <jmtorres@ull.es>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <DeckLinkAPI.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include "ComPtr.h"
#include "DeckLinkOpenCv.h"

using namespace boost;

namespace logging = boost::log;

class CvMatDeckLinkVideoFrame : public IDeckLinkVideoFrame {
public:
    cv::Mat mat;

    CvMatDeckLinkVideoFrame(int row, int cols)
            : mat(row, cols, CV_8UC4) { }

    //
    // IDeckLinkVideoFrame
    //

    long GetWidth() { return mat.rows; }

    long GetHeight() { return mat.cols; }

    long GetRowBytes() { return mat.step; }

    BMDPixelFormat GetPixelFormat() { return bmdFormat8BitBGRA; }

    BMDFrameFlags GetFlags() { return 0; }

    HRESULT GetBytes(void **buffer) {
        *buffer = mat.data;
        return S_OK;
    }

    HRESULT GetTimecode(BMDTimecodeFormat format,
                        IDeckLinkTimecode **timecode) {
        *timecode = nullptr;
        return S_OK;
    }

    HRESULT GetAncillaryData(IDeckLinkVideoFrameAncillary **ancillary) {
        *ancillary = nullptr;
        return S_OK;
    }

    //
    // IDeckLinkVideoFrame
    //

    HRESULT QueryInterface(REFIID iid, LPVOID *ppv) { return E_NOINTERFACE; }

    ULONG AddRef() {
        mat.addref();
        return mat.u->refcount;
    }

    ULONG Release() {
        mat.release();
        if (mat.u->refcount == 0) delete this;
        return mat.u->refcount;
    }
};

bool deckLinkVideoFrameToCvMat(ComPtr<IDeckLinkVideoInputFrame> in,
                               cv::Mat &out) {
    BOOST_LOG_TRIVIAL(info) << "Printing out input: " << in << std::endl;


    switch (in->GetPixelFormat()) {
        case bmdFormat8BitYUV: {
            void *data;

            bool stat = FAILED(in->GetBytes(&data));

            if (FAILED(in->GetBytes(&data)))
                return false;

            cv::Mat mat = cv::Mat(in->GetHeight(), in->GetWidth(), CV_8UC2, data,
                                  in->GetRowBytes());

            cv::cvtColor(mat, out, CV_YUV2BGR_UYVY);
            

            BOOST_LOG_TRIVIAL(info) << "Prining out channels: " << mat.channels() << std::endl;

            return true;
        }
        case bmdFormat8BitBGRA: {
            void *data;
            if (FAILED(in->GetBytes(&data)))
                return false;

            cv::Mat mat = cv::Mat(in->GetHeight(), in->GetWidth(), CV_8UC4, data);
            cv::cvtColor(mat, out, CV_BGRA2BGR);
            return true;
        }
        default: {
            ComPtr<IDeckLinkVideoConversion> deckLinkVideoConversion =
                    CreateVideoConversionInstance();
            if (!deckLinkVideoConversion)
                return false;

            CvMatDeckLinkVideoFrame cvMatWrapper(in->GetHeight(), in->GetWidth());
            if (FAILED(deckLinkVideoConversion->ConvertFrame(in.get(), &cvMatWrapper)))
                return false;
            cv::cvtColor(cvMatWrapper.mat, out, CV_BGRA2BGR);
            return true;
        }
    }
}
