interface HTMLInputEvent extends Event {
  target: HTMLInputElement & EventTarget;
}
export default function Menu() {
  
  const handleFileUpload = async (event: React.ChangeEvent<HTMLInputElement>) => {
    const file: any = event?.target.files && event.target.files[0];
    console.log(file);
    
    if (file) {
      // Convert file to ArrayBuffer
      const arrayBuffer = await file.arrayBuffer();
      console.log(file.name);

      // Pass arrayBuffer to WebAssembly module
      //window._processFile(arrayBuffer, arrayBuffer.byteLength, 'junk');
      window._processFile('junk');


      console.log('done');
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
