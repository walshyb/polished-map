import { MenuItem, Icon, Menu } from "semantic-ui-react";
import { useState } from "react";
import { useAppDispatch } from "../../hooks";
import { toggleExplorer } from "../../store/fileSlice";

export default function ActivityBar({
  fileExplorerOpen,
}: {
  fileExplorerOpen: boolean;
}) {
  const [activeItem, setActiveItem] = useState<string>("file explorer");
  const dispatch = useAppDispatch();

  const handleItemClick = (name: string) => {
    // If curernt item is already active
    if (activeItem === name) {
      dispatch(toggleExplorer());
    }

    setActiveItem(name);
  };

  return (
    <Menu
      icon="labeled"
      inverted
      vertical
      floated={true}
      fixed="left"
      style={{
        display: "flex",
        height: "100%",
        marginTop: "40px",
      }}
    >
      <MenuItem
        name="file explorer"
        active={activeItem === "file explorer"}
        onClick={() => {
          handleItemClick("file explorer");
        }}
      >
        <Icon name="file" />
      </MenuItem>

      <MenuItem
        name="search"
        active={activeItem === "search"}
        onClick={() => {
          handleItemClick("search");
        }}
      >
        <Icon name="search" />
      </MenuItem>
    </Menu>
  );
}
