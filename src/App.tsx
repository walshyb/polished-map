import './App.css';
import Editor from './components/editor/Editor';
import TopBar from './components/menus/TopBar';
import 'semantic-ui-css/semantic.min.css'
import ActivityBar from './components/menus/ActivityBar';
import { useState } from 'react';
import { Grid, Menu, GridColumn, Sidebar, Segment, SidebarPushable, SidebarPusher } from 'semantic-ui-react';
import FileExplorer from './components/menus/FileExplorer';

function App() {
  // Really this is a flag for .(a)blk processed
  const [fileProcessed, setFileProcessed] = useState(false);
  const [fileExplorerActive, setFileExplorerActive] = useState<boolean>(false);

  return (
    <div className="App">
      <TopBar
        fileProcessed={fileProcessed}
        setFileProcessed={setFileProcessed}
      />
      <ActivityBar
        fileExplorerActive={fileExplorerActive}
        setFileExplorerActive={setFileExplorerActive}
      />

      <SidebarPushable as={Segment} style={{ marginLeft: '100px' }}>
        <FileExplorer
          fileExplorerActive={fileExplorerActive}
        />
        <SidebarPusher style={{ minHeight: '100vh', transform: fileExplorerActive ? 'translateX(250px)' : 'none', transition: 'transform 0.5s ease'  }}>
          <Segment basic>
          Content
            <Editor fileProcessed={fileProcessed} />
          </Segment>
        </SidebarPusher>
      </SidebarPushable>
    </div>
  );
}

export default App;
