/*

This file is part of the Rapidly SDK for cross-platform model inference.
Copyright (c) 2026 Rapidly Labs AS.

You are not allowed to use, distribute or modify this code without
a written permission from Rapidly Labs AS.

*/

//
//  RapidlyEngine.kt
//  RapidlyEngine -- Kotlin / Android binding
//
//  Kotlin-native API for the Rapidly SDK. Ships as the rapidly-engine
//  Android library (.aar) with Maven coordinates io.rapidly:engine.
//
//  This is the idiomatic class layer of the binding. It holds no logic of
//  its own: every method is a one-line delegation to the JNI shim
//  (libRapidlyEngineJni.so), which marshals to the stable public C API,
//  which runs the C++ engine. It mirrors the Swift binding's RapidlyEngine
//  surface and semantics one-to-one, expressed in Kotlin idiom (see the
//  SDK language-bindings architecture).
//

package io.rapidly.engine

import java.nio.FloatBuffer

/**
 * Kotlin-native audio processor wrapping the Rapidly C engine.
 *
 * ## Threading (read this before shipping)
 *
 * A [RapidlyEngine] instance is **not safe to call concurrently from
 * multiple threads** at any moment. It follows the same concurrency model
 * as Android's real-time audio types (`AudioTrack`, `AudioRecord`, an Oboe
 * stream callback): the caller is responsible for serialising access.
 *
 * Apps that feed audio from a real-time callback thread should follow this
 * exact stop sequence:
 *
 * 1. Stop your audio stream / callback first. After this returns, the audio
 *    thread is guaranteed not to be inside `addAudio*`, `getAudio*`,
 *    [getProcessorInfo], etc.
 * 2. Call [close] on this engine.
 * 3. Release the engine reference.
 *
 * **Calling [close] while another thread is mid-call into the same engine
 * is undefined behaviour.** It can crash, corrupt memory, or behave
 * unpredictably. This is consistent with how every real-time audio API on
 * Android behaves; coordinating thread lifecycle is a caller responsibility,
 * not the SDK's.
 *
 * ## Lifetime
 *
 * Construction loads the model file and (per the global licence pool at
 * that moment) bakes in the watermarked-or-not state for the lifetime of
 * the instance. Call [addLicense] BEFORE construction to avoid watermarking.
 *
 * [RapidlyEngine] implements [AutoCloseable]. For real-time audio the
 * dominant use case explicit [close] is effectively required: it frees the
 * native processor and the model memory it holds. There is no deterministic
 * finalizer (unlike the Swift binding's ARC `deinit`, the JVM offers no
 * equivalent), so a forgotten [close] leaks the native processor until
 * process exit. For short-lived offline processing prefer Kotlin's
 * [use][kotlin.io.use]:
 *
 * ```
 * RapidlyEngine.create(modelPath, numOfChannels = 1, sampleRate = 48000.0)
 *     ?.use { engine -> /* ... */ }
 * ```
 *
 * ## Construction failure
 *
 * The [create] factories return `null` only when a model file is missing
 * or unreadable. Licence coverage never causes a `null` result: a model
 * your licence does not cover (or any model loaded with no licence) still
 * constructs successfully, but its output is watermarked. This mirrors the
 * Swift binding's failable initialiser, where a missing model yields `nil`.
 */
public class RapidlyEngine private constructor(private var handle: Long) : AutoCloseable {

    /** Information about a neural-network audio processor, from [getProcessorInfo]. */
    public data class ProcessorInfo(
        /**
         * Sample rate the model was trained at. The processor automatically
         * converts sample rates to match the model.
         */
        val sampleRate: Double,
        /**
         * The true number of channels used in the processing. The processor
         * automatically converts the channel format to match the model.
         */
        val numOfModelChannels: Int,
        /** The maximum latency of the model, in samples. */
        val latencyInSamples: Int,
    )

    /** Processor parameters readable with [getParameterValue] and writable with [setParameter]. */
    public enum class Parameters(public val value: Int) {
        /** The maximum attenuation of the unwanted signal in dB. The valid value range is <-inf, 0]. */
        MAXIMUM_ATTENUATION(0x0001),

        /**
         * The sensitivity of the processing in percent ranging from -100% to
         * 100%. 0% is neutral and positive values increase the amount of reduction.
         */
        SENSITIVITY(0x0002),

        /**
         * Enables or disables mask frequency extrapolation. When using models
         * that are not trained on the full frequency band, this setting
         * determines if the output audio should be band-limited to the
         * frequency range of the model or if the high-frequency masks should
         * be extrapolated. Settings below 0.5 disable extrapolation, settings
         * equal to or above 0.5 enable it.
         */
        MASK_EXTRAPOLATION(0x0003),

        /**
         * The stem-dependent sensitivities of the processing in percent
         * ranging from -100% to 100%. 0% is neutral and positive values
         * increase the amount of reduction. Address an individual stem by
         * adding its model index to this value, so [FIRST_MODEL_SENSITIVITY]
         * is the first model, `FIRST_MODEL_SENSITIVITY.value + 1` the second,
         * and so forth.
         */
        FIRST_MODEL_SENSITIVITY(0x1000),
    }

    /**
     * Adds floating-point PCM encoded audio from separate channels to the processor.
     *
     * @param pcmChannels One direct [FloatBuffer] per channel, each holding
     *   sampled PCM values. Buffers must be direct (`ByteBuffer.allocateDirect`
     *   based) so the audio crosses to native code without a copy; audio is
     *   read starting at element 0.
     * @param numOfSamples The number of samples to add, per channel.
     */
    public fun addAudio(pcmChannels: Array<FloatBuffer>, numOfSamples: Int) {
        nativeAddAudio(handle, pcmChannels, numOfSamples)
    }

    /**
     * Adds floating-point PCM encoded audio from a single channel-interleaved buffer.
     *
     * @param pcmChannels A direct [FloatBuffer] of channel-interleaved PCM
     *   audio (stereo audio is "Left Sample 1", "Right Sample 1",
     *   "Left Sample 2", ...). Must be direct so the audio crosses to native
     *   code without a copy; audio is read starting at element 0.
     * @param numOfSamples The number of samples to add, per channel.
     */
    public fun addAudioInterleaved(pcmChannels: FloatBuffer, numOfSamples: Int) {
        nativeAddAudioInterleaved(handle, pcmChannels, numOfSamples)
    }

    /**
     * Returns the number of samples ready after model inference.
     *
     * Processing introduces latency, so query this before retrieving audio
     * with [getAudio] / [getAudioInterleaved].
     *
     * @return The number of completed samples.
     */
    public fun getNumOfPendingSamples(): Int = nativeGetNumOfPendingSamples(handle)

    /**
     * Gets floating-point PCM encoded audio in separate channels after processing.
     *
     * [numOfSamples] must be less than or equal to the number of available
     * samples returned by [getNumOfPendingSamples].
     *
     * @param pcmChannels One direct [FloatBuffer] per channel, each receiving
     *   sampled PCM values. Buffers must be direct; audio is written starting
     *   at element 0.
     * @param numOfSamples The number of samples to retrieve, per channel.
     * @return `true` on success, otherwise `false`.
     */
    public fun getAudio(pcmChannels: Array<FloatBuffer>, numOfSamples: Int): Boolean =
        nativeGetAudio(handle, pcmChannels, numOfSamples)

    /**
     * Gets floating-point PCM encoded audio in a single channel-interleaved
     * buffer after processing.
     *
     * [numOfSamples] must be less than or equal to the number of available
     * samples returned by [getNumOfPendingSamples].
     *
     * @param pcmChannels A direct [FloatBuffer] receiving channel-interleaved
     *   PCM audio. Must be direct; audio is written starting at element 0.
     * @param numOfSamples The number of samples to retrieve, per channel.
     * @return `true` on success, otherwise `false`.
     */
    public fun getAudioInterleaved(pcmChannels: FloatBuffer, numOfSamples: Int): Boolean =
        nativeGetAudioInterleaved(handle, pcmChannels, numOfSamples)

    /** Resets the processor state and clears all delay lines. */
    public fun resetProcessorState() {
        nativeResetProcessorState(handle)
    }

    /**
     * Returns information about the Rapidly processor.
     *
     * @return A [ProcessorInfo] describing the processor.
     */
    public fun getProcessorInfo(): ProcessorInfo {
        val info = nativeGetProcessorInfo(handle)
        return ProcessorInfo(
            sampleRate = info[0],
            numOfModelChannels = info[1].toInt(),
            latencyInSamples = info[2].toInt(),
        )
    }

    /**
     * Returns a parameter value.
     *
     * @param parameter One of the [Parameters].
     * @return The current parameter value.
     */
    public fun getParameterValue(parameter: Parameters): Float =
        nativeGetParameterValue(handle, parameter.value)

    /**
     * Sets a parameter value.
     *
     * @param parameter One of the [Parameters].
     * @param value The new parameter value.
     */
    public fun setParameter(parameter: Parameters, value: Float) {
        nativeSetParameterValue(handle, parameter.value, value)
    }

    /**
     * Releases the underlying processor, freeing its resources. Safe to call
     * multiple times. After [close], all audio methods become no-ops and read
     * methods return `false` / zero.
     *
     * For real-time audio, calling this from your audio-stop path BEFORE
     * releasing references to the engine is effectively required: it avoids a
     * race where the audio thread is still inside `addAudio` / `getAudio` when
     * the processor is freed. See the threading notes on [RapidlyEngine].
     */
    public override fun close() {
        if (handle != 0L) {
            nativeDeleteProcessor(handle)
            handle = 0L
        }
    }

    public companion object {

        init {
            // Load the engine library directly FIRST: loading through the JVM
            // (rather than leaving it to the dynamic linker as a NEEDED
            // dependency of the JNI shim) makes the JVM invoke the engine's
            // JNI_OnLoad, which the engine needs to reach the platform
            // networking stack for online licence keys. Offline licence keys
            // work either way. Then load the JNI shim; its engine dependency
            // is already satisfied. The .aar packs both .so side by side.
            System.loadLibrary("RapidlyEngine")
            System.loadLibrary("RapidlyEngineJni")
        }

        /**
         * Adds a licence key to the Rapidly SDK.
         *
         * Licence state is global, so this must be called BEFORE constructing
         * any [RapidlyEngine] instance. Without a covering licence the SDK
         * operates in a watermarked demo mode that audibly degrades its
         * output, and that state is baked into each processor at creation
         * time: a later licence call does not retroactively un-watermark
         * already-created processors.
         *
         * @param licenseString A licence string received from Rapidly.
         * @return `true` if the licence check succeeded, otherwise `false`.
         */
        @JvmStatic
        public fun addLicense(licenseString: String): Boolean = nativeAddLicense(licenseString)

        /**
         * This call is ignored because the SDK only ever talks to Rapidly's
         * production API. It does so only when a subscription key or
         * activation key is in use, for licensing and billing. Licence keys
         * are fully offline. Audio never leaves the device.
         */
        @JvmStatic
        public fun setApiBaseUrl(baseUrl: String): Unit = nativeSetApiBaseUrl(baseUrl)

        /**
         * Starts a usage session for one end user. Subscription deployments
         * only; silently ignored otherwise. Use an opaque identifier (a UUID
         * or hash), never an email or name. Non-blocking: audio is never
         * interrupted while the session is established.
         *
         * @return `true` if the SDK is currently authorized for processing.
         */
        @JvmStatic
        public fun startSession(externalUserId: String): Boolean = nativeStartSession(externalUserId)

        /**
         * Ends the current usage session and reports its duration.
         * Subscription deployments only; silently ignored otherwise.
         */
        @JvmStatic
        public fun stopSession(): Unit = nativeStopSession()

        /**
         * Creates an audio processor and loads a single model file.
         *
         * @param modelFilepath File path of the model file to load.
         * @param numOfChannels The number of channels in the audio to process.
         * @param sampleRate The sample rate of the audio to process.
         * @return A [RapidlyEngine] on success. Returns `null` only if the
         *   model file is missing or unreadable. Licence coverage never causes
         *   `null`: a model your licence does not cover (or any model loaded
         *   with no licence) still constructs successfully, but its output is
         *   watermarked.
         */
        @JvmStatic
        public fun create(
            modelFilepath: String,
            numOfChannels: Int,
            sampleRate: Double,
        ): RapidlyEngine? {
            val handle = nativeCreateProcessor(modelFilepath, numOfChannels, sampleRate)
            return if (handle != 0L) RapidlyEngine(handle) else null
        }

        /**
         * Creates a processor that performs stem separation, loading a set of
         * model files. The number of stems is derived from [modelFilepaths].
         *
         * @param modelFilepaths File paths of the model files to load.
         * @param numOfChannels The number of channels in the audio to process.
         * @param sampleRate The sample rate of the audio to process.
         * @return A [RapidlyEngine] on success. Returns `null` only if a model
         *   file is missing or unreadable. Licence coverage never causes
         *   `null`: a model your licence does not cover (or any model loaded
         *   with no licence) still constructs successfully, but its output is
         *   watermarked.
         */
        @JvmStatic
        public fun create(
            modelFilepaths: List<String>,
            numOfChannels: Int,
            sampleRate: Double,
        ): RapidlyEngine? {
            val handle = nativeCreateStemSeparator(
                modelFilepaths.toTypedArray(), numOfChannels, sampleRate,
            )
            return if (handle != 0L) RapidlyEngine(handle) else null
        }

        // --- JNI shim entry points (libRapidlyEngineJni.so) ---------------
        // @JvmStatic hoists each native method onto RapidlyEngine itself, so
        // the JNI symbol is Java_io_rapidly_engine_RapidlyEngine_native*,
        // matching rapidly_jni.cpp. The processor handle is passed explicitly
        // as a Long, so the native side never holds a back-reference to this
        // object.

        @JvmStatic
        private external fun nativeAddLicense(licenseString: String): Boolean

        @JvmStatic
        private external fun nativeSetApiBaseUrl(baseUrl: String)

        @JvmStatic
        private external fun nativeStartSession(externalUserId: String): Boolean

        @JvmStatic
        private external fun nativeStopSession()

        @JvmStatic
        private external fun nativeCreateProcessor(
            modelFilepath: String, numOfChannels: Int, sampleRate: Double,
        ): Long

        @JvmStatic
        private external fun nativeCreateStemSeparator(
            modelFilepaths: Array<String>, numOfChannels: Int, sampleRate: Double,
        ): Long

        @JvmStatic
        private external fun nativeDeleteProcessor(handle: Long)

        @JvmStatic
        private external fun nativeAddAudio(
            handle: Long, pcmChannels: Array<FloatBuffer>, numOfSamples: Int,
        )

        @JvmStatic
        private external fun nativeAddAudioInterleaved(
            handle: Long, interleavedPCM: FloatBuffer, numOfSamples: Int,
        )

        @JvmStatic
        private external fun nativeGetNumOfPendingSamples(handle: Long): Int

        @JvmStatic
        private external fun nativeGetAudio(
            handle: Long, pcmChannels: Array<FloatBuffer>, numOfSamples: Int,
        ): Boolean

        @JvmStatic
        private external fun nativeGetAudioInterleaved(
            handle: Long, interleavedPCM: FloatBuffer, numOfSamples: Int,
        ): Boolean

        @JvmStatic
        private external fun nativeResetProcessorState(handle: Long)

        @JvmStatic
        private external fun nativeGetProcessorInfo(handle: Long): DoubleArray

        @JvmStatic
        private external fun nativeGetParameterValue(handle: Long, parameterIndex: Int): Float

        @JvmStatic
        private external fun nativeSetParameterValue(
            handle: Long, parameterIndex: Int, parameterValue: Float,
        )
    }
}
