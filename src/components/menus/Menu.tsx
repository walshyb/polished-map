import { processFile } from '../../utils/wasm-funcs';

interface HTMLInputEvent extends Event {
  target: HTMLInputElement & EventTarget;
}
export default function Menu() {
  
  const handleFileUpload = async (event: React.ChangeEvent<HTMLInputElement>) => {
    const file: any = event?.target.files && event.target.files[0];
    
    if (file) {
      // Convert file to ArrayBuffer
      const arrayBuffer: ArrayBuffer = await file.arrayBuffer();

      // Pass arrayBuffer to WebAssembly module
      processFile(arrayBuffer, arrayBuffer.byteLength, file.name);
    }
  };

  return (
    <div>
      <ul>
        <li>
          File
          <input
            type="file"
            onChange={handleFileUpload}
          />
        </li>
        <li>Data</li>
        <li>Edit</li>
        <li>View</li>
        <li>Mode</li>
        <li>Tools</li>
        <li>Options</li>
      </ul>
    </div>
  );
}
