function createWasmAudioWorkletProcessor(audioParams){class WasmAudioWorkletProcessor extends AudioWorkletProcessor{constructor(args){super();globalThis.stackAlloc=Module["stackAlloc"];globalThis.stackSave=Module["stackSave"];globalThis.stackRestore=Module["stackRestore"];globalThis.HEAPU32=Module["HEAPU32"];globalThis.HEAPF32=Module["HEAPF32"];let opts=args.processorOptions;this.callbackFunction=Module["wasmTable"].get(opts["cb"]);this.userData=opts["ud"];this.samplesPerChannel=opts["sc"]}static get parameterDescriptors(){return audioParams}process(inputList,outputList,parameters){let numInputs=inputList.length,numOutputs=outputList.length,numParams=0,i,j,k,dataPtr,bytesPerChannel=this.samplesPerChannel*4,stackMemoryNeeded=(numInputs+numOutputs)*12,oldStackPtr=stackSave(),inputsPtr,outputsPtr,outputDataPtr,paramsPtr,didProduceAudio,paramArray;for(i of inputList)stackMemoryNeeded+=i.length*bytesPerChannel;for(i of outputList)stackMemoryNeeded+=i.length*bytesPerChannel;for(i in parameters)stackMemoryNeeded+=parameters[i].byteLength+8,++numParams;inputsPtr=stackAlloc(stackMemoryNeeded);k=inputsPtr>>2;dataPtr=inputsPtr+numInputs*12;for(i of inputList){HEAPU32[k+0]=i.length;HEAPU32[k+1]=this.samplesPerChannel;HEAPU32[k+2]=dataPtr;k+=3;for(j of i){HEAPF32.set(j,dataPtr>>2);dataPtr+=bytesPerChannel}}outputsPtr=dataPtr;k=outputsPtr>>2;outputDataPtr=(dataPtr+=numOutputs*12)>>2;for(i of outputList){HEAPU32[k+0]=i.length;HEAPU32[k+1]=this.samplesPerChannel;HEAPU32[k+2]=dataPtr;k+=3;dataPtr+=bytesPerChannel*i.length}paramsPtr=dataPtr;k=paramsPtr>>2;dataPtr+=numParams*8;for(i=0;paramArray=parameters[i++];){HEAPU32[k+0]=paramArray.length;HEAPU32[k+1]=dataPtr;k+=2;HEAPF32.set(paramArray,dataPtr>>2);dataPtr+=paramArray.length*4}if(didProduceAudio=this.callbackFunction(numInputs,inputsPtr,numOutputs,outputsPtr,numParams,paramsPtr,this.userData)){for(i of outputList){for(j of i){for(k=0;k<this.samplesPerChannel;++k){j[k]=HEAPF32[outputDataPtr++]}}}}stackRestore(oldStackPtr);return!!didProduceAudio}}return WasmAudioWorkletProcessor}class BootstrapMessages extends AudioWorkletProcessor{constructor(arg){super();globalThis.Module=arg["processorOptions"];globalThis.Module["instantiateWasm"]=(info,receiveInstance)=>{var instance=new WebAssembly.Instance(Module["wasm"],info);return receiveInstance(instance,Module["wasm"])};let p=globalThis["messagePort"]=this.port;p.onmessage=async msg=>{let d=msg.data;if(d["_wpn"]){registerProcessor(d["_wpn"],createWasmAudioWorkletProcessor(d["ap"]));p.postMessage({_wsc:d["cb"],x:[d["ch"],1,d["ud"]]})}else if(d["_wsc"]){var ptr=d["_wsc"];Module["wasmTable"].get(ptr)(...d["x"])}}}process(){}}registerProcessor("message",BootstrapMessages);