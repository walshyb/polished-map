import "./App.css";
import Editor from "./components/editor/Editor";
import TopBar from "./components/menus/TopBar";
import "semantic-ui-css/semantic.min.css";
import ActivityBar from "./components/menus/ActivityBar";
import { useState } from "react";
import { Segment, SidebarPushable, SidebarPusher } from "semantic-ui-react";
import FileExplorer from "./components/menus/FileExplorer";
import { useAppSelector } from "./hooks";

function App() {
  // Really this is a flag for .(a)blk processed
  const [fileProcessed, setFileProcessed] = useState(false);
  const fileExplorerOpen = useAppSelector((state) => state.file.explorerOpen);

  return (
    <div
      className="App"
      style={{
        maxHeight: "100vh",
        overflowY: "hidden",
      }}
    >
      <TopBar
        fileProcessed={fileProcessed}
        setFileProcessed={setFileProcessed}
      />
      <ActivityBar fileExplorerOpen={fileExplorerOpen} />

      <SidebarPushable
        as={Segment}
        style={{
          marginLeft: "83px",
          border: 0,
          marginTop: "-15px",
          background: "#939090",
        }}
      >
        <FileExplorer fileExplorerOpen={fileExplorerOpen} />
        <SidebarPusher
          style={{
            minHeight: "100vh",
            transform: fileExplorerOpen ? "translateX(250px)" : "none",
            transition: "transform 0.5s ease",
          }}
        >
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
