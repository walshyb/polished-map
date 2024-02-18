export default function Menu() {
  const handleFileUpload = async (event) => {
    const file = event.target.files[0];
    
    if (file) {
      // Convert file to ArrayBuffer
      const arrayBuffer = await file.arrayBuffer();

      // Pass arrayBuffer to WebAssembly module
      window._processFile(arrayBuffer);

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
