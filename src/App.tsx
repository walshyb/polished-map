import './App.css';
import Editor from './components/editor/Editor';
import TopBar from './components/menus/TopBar';
import 'semantic-ui-css/semantic.min.css'
import { useState } from 'react';

function App() {
  // Really this is a flag for .(a)blk processed
  const [fileProcessed, setFileProcessed] = useState(false);
  console.log(fileProcessed);
  return (
    <div className="App">
      <TopBar
        fileProcessed={fileProcessed}
        setFileProcessed={setFileProcessed}
      />
      <Editor
        fileProcessed={fileProcessed}
      />
    </div>
  );
}

export default App;
