#if defined(__ANDROID__)
#include <jni.h>
#include <android/asset_manager_jni.h>
#include "../proton/resourcemanager.hh"

extern "C" JNIEXPORT void JNICALL
Java_org_libsdl_app_SDLActivity_nativeInit(JNIEnv *env, jobject /* this */, jobject assetManager)
{
    AAssetManager *nativeAssetManager = AAssetManager_fromJava(env, assetManager);

    Proton::ResourceManager::getInstance().setAssetManager(nativeAssetManager);
}
#endif