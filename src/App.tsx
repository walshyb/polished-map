import './App.css';
import Editor from './components/editor/Editor';
import TopBar from './components/menus/TopBar';
import 'semantic-ui-css/semantic.min.css'

function App() {
  return (
    <div className="App">
      <TopBar />
      <Editor />
    </div>
  );
}

export default App;
