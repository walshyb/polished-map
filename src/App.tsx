import './App.css';
import Editor from './components/editor/Editor';
import TopBar from './components/menus/TopBar';
import 'semantic-ui-css/semantic.min.css'
import ActivityBar from './components/menus/ActivityBar';
import { useState } from 'react';
import { Grid, Menu, GridColumn } from 'semantic-ui-react';
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
      <Grid>
        <GridColumn width={4} style={{ paddingTop: 0, position: 'fixed', height: '100vh', overflowY: 'auto' }}>
          <ActivityBar
            fileExplorerActive={fileExplorerActive}
            setFileExplorerActive={setFileExplorerActive}
          />
          {/* Actually this should be a secondary menu in which the file explorer is one option */}
          {/* But later TODO */}
          <FileExplorer
            fileExplorerActive={fileExplorerActive}
          />
        </GridColumn>

        <GridColumn width={12} style={{ marginLeft: '25%', paddingTop: '50px' }}>
          <Editor fileProcessed={fileProcessed} />
        </GridColumn>
      </Grid>
    </div>
  );
}

export default App;
