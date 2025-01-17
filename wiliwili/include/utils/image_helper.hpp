//
// Created by fang on 2022/7/16.
//

#pragma once

#include <borealis.hpp>
#include <cpr/cpr.h>
#include <ctime>
#include <random>
#include <unordered_map>

/**
 * 图片加载请求，每个请求对应一个ImageHelper的实例
 */
class ImageHelper {
    typedef std::list<std::shared_ptr<ImageHelper>> Pool;

public:
    ImageHelper(brls::Image* view);

    virtual ~ImageHelper();

    /**
     * 取消图片请求
     */
    void cancel();

    void setImageView(brls::Image* view);

    brls::Image* getImageView();

    /**
     * 设置要加载内容的图片组件。此函数需要工作在主线程。
     */
    static std::shared_ptr<ImageHelper> with(brls::Image* view);

    /**
     * 加载网络图片。此函数需要工作在主线程。
     */
    void load(std::string url);

    /**
     * 取消请求，并清空图片。此函数需要工作在主线程。
     */
    static void clear(brls::Image* view);

    static void setRequestThreads(size_t num);

    /// 图片请求后缀，用来控制图片大小
    inline static std::string h_ext    = "@672w_378h_1c.jpg";
    inline static std::string v_ext    = "@312w_420h_1c.jpg";
    inline static std::string face_ext = "@96w_96h_1c_1s.jpg";

    /// 图片请求线程数
    inline static size_t REQUEST_THREADS = 1;

protected:
    virtual void requestImage();

    /**
     * 图片请求结束时调用
     */
    void clean();

private:
    bool isCancel;
    brls::Image* imageView;
    std::string imageUrl;
    Pool::iterator currentIter;

    /// 清理图片或取消请求时，用来定位 ImageHelper
    inline static std::unordered_map<brls::Image*, Pool::iterator> requestMap;

    /// 请求队列，可复用其中的 ImageHelper
    inline static Pool requestPool;
    inline static std::mutex requestMutex;
};