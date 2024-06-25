import Metatiles from "./Metatiles";
import Map from "./map/Map";
import "./Editor.scss";

interface EditorProps {
  metatiles: any;
}

export default function Editor({ metatiles }: EditorProps) {
  return (
    <div className="editor">
      <Metatiles metatiles={metatiles} />
      <Map metatiles={metatiles} />
    </div>
  );
}
